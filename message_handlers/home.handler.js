'use strict';

const config = require('../config.js');

var home_custom_functions = [];

home_custom_functions.push({
    specifier: 'Home',
    name: 'getVersionNumber',
    function: (component, addons, socket, parameters) => {
        return config.version;
    }
});

module.exports = home_custom_functions;
