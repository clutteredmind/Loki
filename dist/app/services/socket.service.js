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
var SocketService = (function () {
    function SocketService() {
        this.addons = new Array();
        this.pending_messages = new Array();
        var host_string = location.host.indexOf(':') ? location.host.substr(0, location.host.indexOf(':')) : location.host;
        this.connectToServer('ws://' + host_string + ':8889');
    }
    SocketService.prototype.register = function (addon) {
        var index = this.addons.push(addon);
        this.sendMessage({
            category: 'system',
            action: 'register',
            data: addon.category
        });
    };
    SocketService.prototype.unregister = function (addonToRemove) {
        var _this = this;
        this.sendMessage({
            category: 'system',
            action: 'unregister',
            data: addonToRemove.category
        });
        this.addons.forEach(function (addon, index) {
            if (addonToRemove == addon) {
                _this.addons.splice(index, 1);
            }
        });
    };
    SocketService.prototype.sendMessage = function (message) {
        var self = this;
        if (this.socket.readyState != WebSocket.OPEN) {
            this.pending_messages.push(message);
            setTimeout(function () { self.clearPendingMessageQueue(); }, 500);
        }
        else {
            this.socket.send(JSON.stringify(message));
        }
    };
    SocketService.prototype.clearPendingMessageQueue = function () {
        var _this = this;
        if (this.pending_messages.length > 0 && this.socket.readyState == WebSocket.OPEN) {
            this.pending_messages.forEach(function (message) {
                _this.socket.send(JSON.stringify(message));
            });
        }
    };
    SocketService.prototype.ngOnDestroy = function () {
        this.socket.close();
    };
    SocketService.prototype.connectToServer = function (serverAddress) {
        var self = this;
        if (self.socket == undefined || self.socket.readyState == WebSocket.CLOSED || self.socket.readyState == WebSocket.CLOSING) {
            self.socket = null;
            self.socket = new WebSocket(serverAddress);
            self.socket.addEventListener('message', function (event) {
                var data = JSON.parse(event.data);
                self.addons.forEach(function (addon) {
                    if (addon.category == data.category) {
                        addon.processMessage(data);
                    }
                });
            });
            self.socket.onclose = function (event) {
                setTimeout(function () { self.connectToServer(serverAddress); }, 5000);
            };
        }
    };
    return SocketService;
}());
SocketService = __decorate([
    core_1.Injectable(),
    __metadata("design:paramtypes", [])
], SocketService);
exports.SocketService = SocketService;
//# sourceMappingURL=socket.service.js.map