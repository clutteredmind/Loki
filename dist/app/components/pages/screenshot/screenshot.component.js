"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
var core_1 = require("@angular/core");
var socket_service_1 = require("../../../services/socket.service");
var ScreenshotComponent = (function () {
    function ScreenshotComponent(socketService) {
        this.socketService = socketService;
        this.category = 'screenshot';
        this.errors = new Array();
        this.image_name = undefined;
        this.images = new Array();
        socketService.register(this);
    }
    ScreenshotComponent.prototype.getScreenshot = function () {
        this.screenshot_loading = true;
        this.socketService.sendMessage({
            category: 'screenshot',
            action: 'captureScreen',
            data: undefined
        });
    };
    ScreenshotComponent.prototype.processMessage = function (message) {
        switch (message.action) {
            case 'captureScreen':
                this.image_name = message.data.image_name;
                this.screenshot_loading = false;
                this.socketService.sendMessage({
                    category: 'screenshot',
                    action: 'getScreenshotList',
                    data: undefined
                });
                break;
            case 'getScreenshotList':
                if (message.data) {
                    this.images = message.data.image_list;
                }
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    };
    ScreenshotComponent.prototype.ngOnInit = function () {
        this.socketService.sendMessage({
            category: 'screenshot',
            action: 'getScreenshotList',
            data: undefined
        });
    };
    ScreenshotComponent.prototype.ngOnDestroy = function () {
        this.socketService.unregister(this);
    };
    return ScreenshotComponent;
}());
ScreenshotComponent = __decorate([
    core_1.Component({
        moduleId: module.id,
        selector: 'screenshot',
        templateUrl: './screenshot.component.html'
    }),
    __metadata("design:paramtypes", [socket_service_1.SocketService])
], ScreenshotComponent);
exports.ScreenshotComponent = ScreenshotComponent;
//# sourceMappingURL=screenshot.component.js.map