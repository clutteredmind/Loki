const deviceListAddon = require('./build/Release/DeviceListAddon');
const systemInformationAddon = require('./build/Release/SystemInformationAddon');
const processListAddon = require('./build/Release/ProcessListAddon');

console.log(`DeviceListAddon`);
try {
  console.log(deviceListAddon.GetDevices());
  console.log(deviceListAddon.GetAddonVersion());
} catch (err) {
  console.log(`caught an error`);
  console.log(err);
}

console.log(`SystemInformationAddon`);
try {
  console.log(systemInformationAddon.GetMemoryInformation());
  console.log(systemInformationAddon.GetCpuInformation());
  console.log(systemInformationAddon.GetAddonVersion());
} catch (err) {
  console.log(`caught an error`);
  console.log(err);
}

console.log(`ProcessListAddon`);
try {
  console.log(processListAddon.GetProcesses());
  console.log(processListAddon.GetProcessModules(8564));
  console.log(processListAddon.GetAddonVersion());
} catch (err) {
  console.log(`caught an error`);
  console.log(err);
}