import { Component, OnInit, OnDestroy } from '@angular/core';

import { Addon }                        from '../../../interfaces/addon.interface';
import { Device }                       from '../../../interfaces/device.interface';
import { SocketMessage }                from '../../../interfaces/socket-message.interface';

import { SocketService }                from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'device-list',
    templateUrl: './device-list.component.html'
})
export class DeviceListComponent implements OnInit, OnDestroy, Addon {
    component = 'device-list';
    errors: Array<string>;
    devices: Array<Device>;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.devices = new Array<Device>();
        socketService.register(this);
    }

    getDeviceList(): void {
        this.socketService.sendMessage({
            component: this.component,
            specifier: undefined,
            action: 'getDevices',
            data: undefined
        });
    }

    processMessage(message: SocketMessage): void {
        switch(message.action) {
            case 'getDevices':
                this.devices = message.data;
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    }

    ngOnInit() {
        this.getDeviceList();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
