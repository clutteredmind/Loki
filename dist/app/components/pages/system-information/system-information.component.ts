import { Component, OnInit, OnDestroy } from '@angular/core';

import { LokiComponent }                from '../../../interfaces/loki-component.interface';
import { SocketMessage }                from '../../../interfaces/socket-message.interface';

import { SocketService }                from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'system-information',
    styles: [`
        .refresh-checkbox {
            margin-left: 10px;
            margin-right: 2px;
        }
        `],
    templateUrl: './system-information.component.html'
})
export class SystemInformationComponent implements OnInit, OnDestroy, LokiComponent {
    component = 'system-information';
    errors: Array<string>;
    memoryInfo = {};
    cpuInfo = {};
    refreshTimer: NodeJS.Timer;

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

    toggleAutoRefresh(checkState: boolean): void {
        if(checkState) {
            // set a timer to request memory information once per second
            this.refreshTimer = setInterval(() => {
                this.socketService.sendMessage({
                    component: this.component,
                    specifier: undefined,
                    action: 'getMemoryInformation',
                    data: undefined
                });
            }, 1000);
        } else {
            // cancel timer
            clearTimeout(this.refreshTimer);
        }
    }

    ngOnInit() {
        this.socketService.register(this);
        this.getMemoryInfo();
        this.getCpuInfo();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
        // clear timer, in case it exists
        clearTimeout(this.refreshTimer);
    }
}
