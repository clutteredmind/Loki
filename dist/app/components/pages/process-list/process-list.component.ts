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
    category = 'process-list';
    errors: Array<string>;
    processes: Array<Process>;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.processes = new Array<Process>();
        socketService.register(this);
    }

    getProcessList(): void {
        this.socketService.sendMessage({
            category: 'process-list',
            action: 'getProcesses',
            data: undefined
        });
    }

    getProcessModules(processId: number): void {
        console.log('process id: ' + processId);
        this.socketService.sendMessage({
            category: 'process-list',
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
