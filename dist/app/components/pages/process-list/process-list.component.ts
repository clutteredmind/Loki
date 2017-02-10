import { Component, OnInit, OnDestroy } from '@angular/core';

import { Addon }                        from '../../../interfaces/addon.interface';
import { Process }                      from '../../../interfaces/process.interface';
import { SocketMessage }                from '../../../interfaces/socket-message.interface';

import { SocketService }                from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'process-list',
    templateUrl: './process-list.component.html',
    styles: ['.get-modules-link { cursor: pointer; }']
})
export class ProcessListComponent implements OnInit, OnDestroy, Addon {
    component = 'process-list';
    errors: Array<string>;
    processes: Array<Process>;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.processes = new Array<Process>();
        socketService.register(this);
    }

    getProcessList(): void {
        this.socketService.sendMessage({
            component: this.component,
            specifier: undefined,
            action: 'getProcesses',
            data: undefined
        });
    }

    getProcessModules(processId: number): void {
        this.socketService.sendMessage({
            component: this.component,
            specifier: undefined,
            action: 'getProcessModules',
            data: undefined,
            parameters: [processId]
        });
    }

    processMessage(message: SocketMessage): void {
        switch(message.action) {
            case 'getProcesses':
                this.processes = message.data;
                break;
            case 'getProcessModules':
                // do nothing with this message
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    }

    ngOnInit() {
        this.getProcessList();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
