import { Component, OnInit, OnDestroy, ViewChild } from '@angular/core';

import { BsModalComponent } from 'ng2-bs3-modal';

import { SocketService } from '../../../services/socket.service';
import { SocketMessage } from '../../../interfaces/socket-message.interface';
import { LokiComponent } from '../../../interfaces/loki-component.interface';

@Component({
    selector: 'app-screenshot',
    templateUrl: './screenshot.component.html',
    styleUrls: ['./screenshot.component.css']
})
export class ScreenshotComponent implements OnInit, OnDestroy, LokiComponent {
    component = 'screenshot';
    errors: Array<string>;
    image_name: string;
    images: Array<string>;
    screenshot_loading: boolean;

    @ViewChild('modal') modal: BsModalComponent;

    constructor(private socketService: SocketService) {
        this.image_name = undefined;
        this.errors = new Array<string>();
        this.images = new Array<string>();
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

    clearScreenshots() {
        this.socketService.sendMessage({
            component: this.component,
            specifier: 'ScreenshotModule',
            action: 'clearScreenshots',
            data: undefined
        });
        // close the modal
        this.modal.close();
    }

    processMessage(message: SocketMessage): void {
        switch (message.action) {
            case 'captureScreen':
                this.image_name = message.data.image_name;
                this.screenshot_loading = false;
                this.getScreenshotList();
                break;
            case 'getScreenshotList':
                if (message.data) {
                    this.images = message.data.image_list;
                }
                break;
            case 'clearScreenshots':
                this.images = new Array<string>();
                this.image_name = "";
                break;
            case 'error':
                this.errors.push(message.data);
                // clear the spinner on any error
                this.screenshot_loading = false;
                break;
        }
    }

    ngOnInit() {
        this.socketService.register(this);
        // query for a list of existing screenshots on startup
        this.getScreenshotList();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
