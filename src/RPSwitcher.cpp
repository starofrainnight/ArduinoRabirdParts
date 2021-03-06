/*
 * RPSwitcher.cpp
 *
 * @date 2016-12-05
 * @author Hong-She Liang <starofrainnight@gmail.com>
 */

#include "RPSwitcher.h"
#include "RPDigitalPin.h"

RPSwitcher::RPSwitcher()
  : mOnLevel(LOW)
  , mStatus(Off)
  , mIsPinManaged(false)
{
}

RPSwitcher::~RPSwitcher()
{
}

void
RPSwitcher::begin(RPDigitalPin *pin, uint8_t onLevel)
{
  end();

  mPin.reset(pin);
  mOnLevel = onLevel;
  off(); // All switcher will be default to off status
}

void
RPSwitcher::begin(uint8_t pin, uint8_t mode, uint8_t onLevel)
{
  end();

  mPin.reset(new RPDigitalPin());
  mPin->begin(pin, mode);

  mOnLevel = onLevel;
  off(); // All switcher will be default to off status

  mIsPinManaged = true;
}

void
RPSwitcher::end()
{
  mOnLevel = LOW;
  mStatus  = Off;

  if(mIsPinManaged)
  {
    mPin->end();

    delete mPin.data();
    mPin.reset();
    mIsPinManaged = false;
  }
}

uint8_t
RPSwitcher::onLevel()
{
  return mOnLevel;
}

void
RPSwitcher::on()
{
  setStatus(On);
}

void
RPSwitcher::off()
{
  setStatus(Off);
}

RPSwitcher::Status
RPSwitcher::status()
{
  if(OUTPUT == mPin->mode())
  {
    return mStatus;
  }
  else
  {
    return ((mPin->read() == onLevel()) ? On : Off);
  }
}

RRawPointer<RPDigitalPin>
RPSwitcher::pin()
{
  return mPin;
}

void
RPSwitcher::setStatus(RPSwitcher::Status status)
{
  if(OUTPUT == mPin->mode())
  {
    if(On == status)
    {
      mPin->write(onLevel());
    }
    else
    {
      mPin->write((onLevel() == HIGH) ? LOW : HIGH);
    }

    mStatus = status;
  }
}
