// this module's category
module.exports.message_category = 'screenshot';

// the name of the C++ addon that goes with this handler
module.exports.module_name = "ScreenshotModule";

// Export a function to handle incoming messages.
module.exports.handle_message = (addon, message) => {
    try {
        switch (message.action) {
            case "captureScreen":
                return addon.captureScreen((data) => {
                    // TODO: not sure what to do here yet
                });
                break;
            case "getModuleInfo":
                return addon.getModuleInfo();
                break;
        }
    } catch (error) {
        // log the error
        console.log("Exception in screenshot handler:");
        console.log(error);
    }
};
