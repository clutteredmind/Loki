// this module's category
module.exports.message_category = 'process-list';

// the name of the C++ addon that goes with this handler
module.exports.module_name = "ProcessListModule";

// Export a function to handle incoming messages.
module.exports.handle_message = (addon, message) => {
    switch (message.action) {
        case "getProcesses":
            return addon.getProcesses();
            break;
        case "getModuleInfo":
            return addon.getModuleInfo();
            break;
        default:
            throw 'No handler for ' + message.action;
            break;
    }
};
