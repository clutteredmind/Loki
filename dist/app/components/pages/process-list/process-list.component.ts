import { Component, OnDestroy } from '@angular/core';

import { Addon }                from '../../../interfaces/addon.interface';
import { Process }              from '../../../interfaces/process.interface';
import { SocketMessage }        from '../../../interfaces/socket-message.interface';

import { SocketService }        from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'process-list',
    templateUrl: './process-list.component.html'
})
export class ProcessListComponent implements OnDestroy, Addon {
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

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
