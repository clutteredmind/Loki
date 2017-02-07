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
var DeviceListComponent = (function () {
    function DeviceListComponent(socketService) {
        this.socketService = socketService;
        this.category = 'device-list';
        this.errors = new Array();
        this.devices = new Array();
        socketService.register(this);
    }
    DeviceListComponent.prototype.getDeviceList = function () {
        this.socketService.sendMessage({
            category: 'device-list',
            action: 'getDevices',
            data: undefined
        });
    };
    DeviceListComponent.prototype.processMessage = function (message) {
        switch (message.action) {
            case 'getDevices':
                this.devices = message.data;
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    };
    DeviceListComponent.prototype.ngOnInit = function () {
        this.socketService.sendMessage({
            category: 'device-list',
            action: 'getDevices',
            data: undefined
        });
    };
    DeviceListComponent.prototype.ngOnDestroy = function () {
        this.socketService.unregister(this);
    };
    return DeviceListComponent;
}());
DeviceListComponent = __decorate([
    core_1.Component({
        moduleId: module.id,
        selector: 'device-list',
        templateUrl: './device-list.component.html'
    }),
    __metadata("design:paramtypes", [socket_service_1.SocketService])
], DeviceListComponent);
exports.DeviceListComponent = DeviceListComponent;
//# sourceMappingURL=device-list.component.js.map