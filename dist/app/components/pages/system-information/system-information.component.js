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
var SystemInformationComponent = (function () {
    function SystemInformationComponent(socketService) {
        this.socketService = socketService;
        this.category = 'system-information';
        this.memoryInfo = {};
        this.cpuInfo = {};
        this.errors = new Array();
    }
    SystemInformationComponent.prototype.processMessage = function (message) {
        switch (message.action) {
            case 'getMemoryInfo':
                this.memoryInfo = message.data;
                break;
            case 'getCpuInfo':
                this.cpuInfo = message.data;
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    };
    SystemInformationComponent.prototype.getMemoryInfo = function () {
        this.socketService.sendMessage({
            category: 'system-information',
            action: 'getMemoryInfo',
            data: undefined
        });
    };
    SystemInformationComponent.prototype.getCpuInfo = function () {
        this.socketService.sendMessage({
            category: 'system-information',
            action: 'getCpuInfo',
            data: undefined
        });
    };
    SystemInformationComponent.prototype.ngOnInit = function () {
        this.socketService.register(this);
        this.socketService.sendMessage({
            category: 'system-information',
            action: 'getMemoryInfo',
            data: undefined
        });
        this.socketService.sendMessage({
            category: 'system-information',
            action: 'getCpuInfo',
            data: undefined
        });
    };
    SystemInformationComponent.prototype.ngOnDestroy = function () {
        this.socketService.unregister(this);
    };
    return SystemInformationComponent;
}());
SystemInformationComponent = __decorate([
    core_1.Component({
        moduleId: module.id,
        selector: 'system-information',
        templateUrl: './system-information.component.html'
    }),
    __metadata("design:paramtypes", [socket_service_1.SocketService])
], SystemInformationComponent);
exports.SystemInformationComponent = SystemInformationComponent;
//# sourceMappingURL=system-information.component.js.map