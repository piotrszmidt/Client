#include "PrintCommand.h"

#include "Global.h"

PrintCommand::PrintCommand(QObject* parent)
    : AbstractCommand(parent),
      channel(Output::DEFAULT_CHANNEL), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), output(Print::DEFAULT_OUTPUT)
{
}

int PrintCommand::getDelay() const
{
    return this->delay;
}

int PrintCommand::getChannel() const
{
    return this->channel;
}

void PrintCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void PrintCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

bool PrintCommand::getAllowGpi() const
{
    return this->allowGpi;
}

void PrintCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

const QString& PrintCommand::getOutput() const
{
    return this->output;
}

void PrintCommand::setOutput(const QString& output)
{
    this->output = output;
    emit outputChanged(this->output);
}

void PrintCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"output") > 0) setOutput(QString::fromStdWString(pt.get<std::wstring>(L"output")));
}

void PrintCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("output", this->getOutput());
}