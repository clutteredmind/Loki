// this module's category
module.exports.message_category = 'device-list';

// the name of the C++ addon that goes with this handler
module.exports.module_name = "DeviceListModule";

// Export a function to handle incoming messages.
module.exports.handle_message = (addon, message) => {
    try {
        switch (message.action) {
            case "getDevices":
                return addon.getDevices();
                break;
            case "getModuleInfo":
                return addon.getModuleInfo();
                break;
        }
    } catch (error) {
        // log the error
        console.log("Exception in device-list handler:");
        console.log(error);
    }
};
