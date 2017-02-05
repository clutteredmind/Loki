"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var core_1 = require("@angular/core");
var platform_browser_1 = require("@angular/platform-browser");
var app_component_1 = require("./app.component");
var app_routing_1 = require("./app.routing");
var navbar_component_1 = require("./components/navbar/navbar.component");
var error_display_component_1 = require("./components/error-display/error-display.component");
var device_list_component_1 = require("./components/pages/device-list/device-list.component");
var home_component_1 = require("./components/pages/home/home.component");
var process_list_component_1 = require("./components/pages/process-list/process-list.component");
var screenshot_component_1 = require("./components/pages/screenshot/screenshot.component");
var socket_service_1 = require("./services/socket.service");
var AppModule = (function () {
    function AppModule() {
    }
    return AppModule;
}());
AppModule = __decorate([
    core_1.NgModule({
        imports: [
            platform_browser_1.BrowserModule,
            app_routing_1.routing
        ],
        declarations: [
            app_component_1.AppComponent,
            navbar_component_1.NavbarComponent,
            home_component_1.HomeComponent,
            error_display_component_1.ErrorDisplayComponent,
            device_list_component_1.DeviceListComponent,
            process_list_component_1.ProcessListComponent,
            screenshot_component_1.ScreenshotComponent
        ],
        bootstrap: [
            app_component_1.AppComponent
        ],
        providers: [
            socket_service_1.SocketService
        ]
    })
], AppModule);
exports.AppModule = AppModule;
//# sourceMappingURL=app.module.js.map