import { Component, OnInit, OnDestroy } from '@angular/core';

import { Addon }                        from '../../../interfaces/addon.interface';
import { SocketMessage }                from '../../../interfaces/socket-message.interface';

import { SocketService }                from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'system-information',
    templateUrl: './system-information.component.html'
})
export class SystemInformationComponent implements OnInit, OnDestroy, Addon {
    category = 'system-information';
    errors: Array<string>;
    memoryInfo = {};
    cpuInfo = {};

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
    }

    processMessage(message: SocketMessage): void {
        switch(message.action) {
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
    }

    getMemoryInfo() {
        this.socketService.sendMessage({
            category: 'system-information',
            action: 'getMemoryInfo',
            data: undefined
        });
    }

    getCpuInfo() {
        this.socketService.sendMessage({
            category: 'system-information',
            action: 'getCpuInfo',
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
