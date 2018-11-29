import { Component, OnInit, OnDestroy } from '@angular/core';

import { Device } from '../../../interfaces/device.interface';
import { SocketService } from '../../../services/socket.service';
import { SocketMessage } from '../../../interfaces/socket-message.interface';
import { LokiComponent } from '../../../interfaces/loki-component.interface';

@Component({
    selector: 'app-device-list',
    templateUrl: './device-list.component.html',
    styleUrls: ['./device-list.component.css']
})
export class DeviceListComponent implements OnInit, OnDestroy, LokiComponent {
    component = 'device-list';
    errors: Array<string>;
    devices: Array<Device>;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.devices = new Array<Device>();
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
        switch (message.action) {
            case 'getDevices':
                this.devices = message.data;
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    }

    ngOnInit() {
        this.socketService.register(this);
        this.getDeviceList();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
