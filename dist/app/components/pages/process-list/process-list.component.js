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
var ProcessListComponent = (function () {
    function ProcessListComponent(socketService) {
        this.socketService = socketService;
        this.category = 'process-list';
        this.errors = new Array();
        this.processes = new Array();
        socketService.register(this);
    }
    ProcessListComponent.prototype.getProcessList = function () {
        this.socketService.sendMessage({
            category: 'process-list',
            action: 'getProcesses',
            data: undefined
        });
    };
    ProcessListComponent.prototype.processMessage = function (message) {
        switch (message.action) {
            case 'getProcesses':
                this.processes = message.data;
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    };
    ProcessListComponent.prototype.ngOnInit = function () {
        this.socketService.sendMessage({
            category: 'process-list',
            action: 'getProcesses',
            data: undefined
        });
    };
    ProcessListComponent.prototype.ngOnDestroy = function () {
        this.socketService.unregister(this);
    };
    return ProcessListComponent;
}());
ProcessListComponent = __decorate([
    core_1.Component({
        moduleId: module.id,
        selector: 'process-list',
        templateUrl: './process-list.component.html'
    }),
    __metadata("design:paramtypes", [socket_service_1.SocketService])
], ProcessListComponent);
exports.ProcessListComponent = ProcessListComponent;
//# sourceMappingURL=process-list.component.js.map