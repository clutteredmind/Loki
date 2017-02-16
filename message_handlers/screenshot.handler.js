'use strict';

var fs = require('fs');
var path = require('path');
var uuid = require('uuid/v1');

var screenshot_custom_functions = [];

screenshot_custom_functions.push({
    specifier: 'ScreenshotModule',
    name: 'captureScreen',
    function: (component, addons, socket, parameters) => {
        // using UUID here to give the image a basically-random name
        var image_name = uuid() + '.jpg';
        var image_file_folder = './dist/images';
        addons.forEach((addon) => {
            addon.functions.forEach((function_name) => {
                if(function_name == 'captureScreen') {
                    // check to see if image directory exists
                    try {
                        fs.readdirSync(image_file_folder);
                    } catch (error) {
                        if (error.code == 'ENOENT') {
                            // the directory does not exist, so create it
                            fs.mkdirSync(image_file_folder);
                        }
                    }
                    // capture screen. we need to go one directory up from where we're at before we append the final directory
                    addon.addon.captureScreen(path.join(__dirname, '../',  image_file_folder, image_name));
                }
            });
        });
        return { image_name: image_name };
    }
});

screenshot_custom_functions.push({
    specifier: 'ScreenshotModule',
    name: 'getScreenshotList',
    function: (component, addons, socket, parameters) => {
        var image_file_folder = './dist/images';
        // check to see if image directory exists
        try {
            fs.readdirSync(image_file_folder);
        } catch (error) {
            if (error.code == 'ENOENT') {
                // the directory does not exist, so create it
                fs.mkdirSync(image_file_folder);
            }
        }
        return { image_list: fs.readdirSync(image_file_folder) };
    }
});

module.exports = screenshot_custom_functions;
