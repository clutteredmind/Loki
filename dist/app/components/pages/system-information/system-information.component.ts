import { Component, OnInit, OnDestroy } from '@angular/core';

import { LokiComponent }                from '../../../interfaces/loki-component.interface';
import { SocketMessage }                from '../../../interfaces/socket-message.interface';

import { SocketService }                from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'system-information',
    templateUrl: './system-information.component.html'
})
export class SystemInformationComponent implements OnInit, OnDestroy, LokiComponent {
    component = 'system-information';
    errors: Array<string>;
    memoryInfo = {};
    cpuInfo = {};

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
    }

    processMessage(message: SocketMessage): void {
        switch(message.action) {
            case 'getMemoryInformation':
                this.memoryInfo = message.data;
                break;
            case 'getCpuInformation':
                this.cpuInfo = message.data;
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    }

    getMemoryInfo() {
        this.socketService.sendMessage({
            component: this.component,
            specifier: undefined,
            action: 'getMemoryInformation',
            data: undefined
        });
    }

    getCpuInfo() {
        this.socketService.sendMessage({
            component: this.component,
            specifier: undefined,
            action: 'getCpuInformation',
            data: undefined
        });
    }

    ngOnInit() {
        this.socketService.register(this);
        this.getMemoryInfo();
        this.getCpuInfo();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
