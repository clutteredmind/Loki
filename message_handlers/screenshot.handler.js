var fs = require('fs');
var path = require('path');
var uuid = require('uuid/v1');

// this module's category
module.exports.message_category = 'screenshot';

// the name of the C++ addon that goes with this handler
module.exports.module_name = "ScreenshotModule";

// Export a function to handle incoming messages.
module.exports.handle_message = (addon, message) => {
    var image_directory = './dist/images';
    try {
        switch (message.action) {
            case "captureScreen":
                // using UUID here to give the image a basically-random name
                var image_name = uuid() + '.jpg';
                // check to see if image directory exists
                try {
                    fs.readdirSync(image_directory);
                } catch (error) {
                    if (error.code == 'ENOENT') {
                        // the directory does not exist, so create it
                        fs.mkdirSync(image_directory);
                    }
                }
                addon.captureScreen((data) => {
                    fs.writeFileSync(path.join(image_directory, image_name), data);
                });
                return { image_name: image_name };
                break;
            case "getScreenshotList":
                return { image_list: fs.readdirSync(image_directory) }
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
