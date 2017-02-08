var fs = require('fs');
var path = require('path');
var uuid = require('uuid/v1');

var function_map = new Map();

function_map.set('captureScreen', (addons, parameters) => {
    // using UUID here to give the image a basically-random name
    var image_name = uuid() + '.jpg';
    var image_file_folder = './dist/images';
    addons.forEach((addon) => {
        for(var key in addon) {
            if(key == 'captureScreen') {
                // check to see if image directory exists
                try {
                    fs.readdirSync(image_file_folder);
                } catch (error) {
                    if (error.code == 'ENOENT') {
                        // the directory does not exist, so create it
                        fs.mkdirSync(image_file_folder);
                    }
                }
                addon.captureScreen((data) => {
                    fs.writeFileSync(path.join(image_file_folder, image_name), data);
                });
            }
        }
    });
    return { image_name: image_name };
});

function_map.set('getScreenshotList', (addons, parameters) => {
    return { image_list: fs.readdirSync('./dist/images') };
});

function_map.set('getProcessModules', (addons, parameters) => {
    // TODO: implement this
    throw new Error('Getting modules for process ' + parameters[0] + ' not implemented yet');
});

module.exports = function_map;
