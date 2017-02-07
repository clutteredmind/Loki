import { Component, OnDestroy, OnInit } from '@angular/core';

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

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        socketService.register(this);
    }

    processMessage(message: SocketMessage): void {
        switch(message.action) {
            case 'getMemoryInfo':
                this.memoryInfo = message.data
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

    ngOnInit() {
        // query for a list of existing screenshots on startup
        this.socketService.sendMessage({
            category: 'screenshot',
            action: 'getScreenshotList',
            data: undefined
        });
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
