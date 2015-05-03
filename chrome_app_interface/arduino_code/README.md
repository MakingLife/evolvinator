connecting to device prompts the device to get updated time and then sleep
currently device will always sleep unless it receives a calibration char
desried behaviour = continue running its thing

code-flow
connect to device
check if there is a run in progress
if yes
  update sensor readings to be broadcast to client
    return options = haltRun, or disconnect
      if haltRun
        return calibrate
if not
  return options = calibrate, or startrun