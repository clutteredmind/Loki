import { Component, OnDestroy, OnInit } from '@angular/core';

import { Addon }                        from '../../../interfaces/addon.interface';
import { SocketMessage }                from '../../../interfaces/socket-message.interface';

import { SocketService }                from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'screenshot',
    templateUrl: './screenshot.component.html'
})
export class ScreenshotComponent implements OnInit, OnDestroy, Addon {
    component = 'screenshot';
    errors: Array<string>;
    image_name: string;
    images: Array<string>;
    screenshot_loading: boolean;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.image_name = undefined;
        this.images = new Array<string>();
        socketService.register(this);
    }

    getScreenshot(): void {
        this.screenshot_loading = true;
        this.socketService.sendMessage({
            component: this.component,
            specifier: 'ScreenshotModule',
            action: 'captureScreen',
            data: undefined
        });
    }

    getScreenshotList(): void {
        this.socketService.sendMessage({
            component: this.component,
            specifier: 'ScreenshotModule',
            action: 'getScreenshotList',
            data: undefined
        });
    }

    processMessage(message: SocketMessage): void {
        switch(message.action) {
            case 'captureScreen':
                this.image_name = message.data.image_name;
                this.screenshot_loading = false;
                // refresh screenshot list
                this.getScreenshotList();
                break;
            case 'getScreenshotList':
                if(message.data) {
                    this.images = message.data.image_list;
                }
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    }

    ngOnInit() {
        // query for a list of existing screenshots on startup
        this.getScreenshotList();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
