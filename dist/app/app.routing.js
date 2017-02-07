"use strict";
var router_1 = require("@angular/router");
var device_list_component_1 = require("./components/pages/device-list/device-list.component");
var home_component_1 = require("./components/pages/home/home.component");
var process_list_component_1 = require("./components/pages/process-list/process-list.component");
var screenshot_component_1 = require("./components/pages/screenshot/screenshot.component");
var system_information_component_1 = require("./components/pages/system-information/system-information.component");
var appRoutes = [
    {
        path: '',
        component: home_component_1.HomeComponent
    },
    {
        path: 'device-list',
        component: device_list_component_1.DeviceListComponent
    },
    {
        path: 'process-list',
        component: process_list_component_1.ProcessListComponent
    },
    {
        path: 'screenshot',
        component: screenshot_component_1.ScreenshotComponent
    },
    {
        path: 'system-information',
        component: system_information_component_1.SystemInformationComponent
    },
    {
        path: '**',
        redirectTo: '/'
    }
];
exports.routing = router_1.RouterModule.forRoot(appRoutes);
//# sourceMappingURL=app.routing.js.map