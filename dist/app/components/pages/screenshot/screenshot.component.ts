import { Component, OnDestroy } from '@angular/core';

import { Addon }                from '../../../interfaces/addon.interface';
import { SocketMessage }        from '../../../interfaces/socket-message.interface';

import { SocketService }        from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'screenshot',
    templateUrl: './screenshot.component.html'
})
export class ScreenshotComponent implements OnDestroy, Addon {
    category = 'screenshot';
    errors: Array<string>;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        socketService.register(this);
    }

    getScreenshot(): void {
        this.socketService.sendMessage({
            category: 'screenshot',
            action: 'captureScreen',
            data: undefined
        });
    }

    processMessage(message: SocketMessage): void {
        switch(message.action) {
            case 'error':
                this.errors.push(message.data);
                break;
        }
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
