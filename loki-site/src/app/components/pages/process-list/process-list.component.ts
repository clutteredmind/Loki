import { Component, OnInit, OnDestroy } from '@angular/core';

import { Process } from '../../../interfaces/process.interface';
import { SocketService } from '../../../services/socket.service';
import { SocketMessage } from '../../../interfaces/socket-message.interface';
import { LokiComponent } from '../../../interfaces/loki-component.interface';

@Component({
    selector: 'app-process-list',
    templateUrl: './process-list.component.html',
    styleUrls: ['./process-list.component.css']
})
export class ProcessListComponent implements OnInit, OnDestroy, LokiComponent {
    component = 'process-list';
    errors: Array<string>;
    processes: Array<Process>;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.processes = new Array<Process>();
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
        switch (message.action) {
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
        this.socketService.register(this);
        this.getProcessList();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
