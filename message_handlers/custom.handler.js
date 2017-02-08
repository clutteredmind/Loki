var fs = require('fs');
var path = require('path');
var uuid = require('uuid/v1');

var function_map = new Map();

function_map.set('captureScreen', function (addons) {
    // using UUID here to give the image a basically-random name
    var image_name = uuid() + '.jpg';
    addons.forEach((addon) => {
        for(var key in addon) {
            if(key == 'captureScreen') {
                // check to see if image directory exists
                try {
                    fs.readdirSync('./dist/images');
                } catch (error) {
                    if (error.code == 'ENOENT') {
                        // the directory does not exist, so create it
                        fs.mkdirSync('./dist/images');
                    }
                }
                addon.captureScreen((data) => {
                    fs.writeFileSync(path.join('./dist/images', image_name), data);
                });
            }
        }
    });
    return { image_name: image_name };
});

function_map.set('getScreenshotList', function () {
    return { image_list: fs.readdirSync('./dist/images') }
});

module.exports = function_map;
