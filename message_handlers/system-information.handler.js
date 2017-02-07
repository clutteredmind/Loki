// this module's category
module.exports.message_category = 'system-information';

// the name of the C++ addon that goes with this handler
module.exports.module_name = "SystemInformationModule";

// Export a function to handle incoming messages.
module.exports.handle_message = (addon, message) => {
    switch (message.action) {
        case 'getMemoryInfo':
            return addon.getMemoryInformation();
            break;
        case 'getCpuInfo':
            return addon.getCpuInformation();
            break;
        case "getModuleInfo":
            return addon.getModuleInfo();
            break;
        default:
            throw 'No handler for ' + message.action;
            break;
    }
};
