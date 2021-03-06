#include "InspectorPresetWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/TriCaster/TriCasterPresetModel.h"

#include <QtGui/QApplication>

InspectorPresetWidget::InspectorPresetWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    loadTriCasterSource();
    loadTriCasterPreset();

    qApp->installEventFilter(this);
}

bool InspectorPresetWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<PresetCommand*>(rundownItemSelectedEvent->getCommand()))
        {  
            this->command = dynamic_cast<PresetCommand*>(rundownItemSelectedEvent->getCommand());

            this->comboBoxSource->setCurrentIndex(this->comboBoxSource->findData(this->command->getSource()));
            this->comboBoxPreset->setCurrentIndex(this->comboBoxPreset->findData(this->command->getPreset()));
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorPresetWidget::blockAllSignals(bool block)
{
    this->comboBoxSource->blockSignals(block);
    this->comboBoxPreset->blockSignals(block);
}

void InspectorPresetWidget::loadTriCasterSource()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxSource->blockSignals(true);

    QList<TriCasterSourceModel> models = DatabaseManager::getInstance().getTriCasterSource();
    foreach (TriCasterSourceModel model, models)
        this->comboBoxSource->addItem(model.getName(), model.getValue());

    this->comboBoxSource->blockSignals(false);
}

void InspectorPresetWidget::loadTriCasterPreset()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxPreset->blockSignals(true);

    QList<TriCasterPresetModel> models = DatabaseManager::getInstance().getTriCasterPreset();
    foreach (TriCasterPresetModel model, models)
        this->comboBoxPreset->addItem(model.getName(), model.getValue());

    this->comboBoxPreset->blockSignals(false);
}

void InspectorPresetWidget::sourceChanged(int index)
{
    this->command->setSource(this->comboBoxSource->itemData(index).toString());
}

void InspectorPresetWidget::presetChanged(int index)
{
    this->command->setPreset(this->comboBoxPreset->itemData(index).toString());
}
