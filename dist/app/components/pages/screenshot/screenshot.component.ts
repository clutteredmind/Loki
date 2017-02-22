import { Component,
         OnDestroy,
         OnInit,
         ViewChild }        from '@angular/core';

import { ModalComponent }   from 'ng2-bs3-modal/ng2-bs3-modal';

import { LokiComponent }    from '../../../interfaces/loki-component.interface';
import { SocketMessage }    from '../../../interfaces/socket-message.interface';

import { SocketService }    from '../../../services/socket.service';

@Component({
    moduleId: module.id,
    selector: 'screenshot',
    templateUrl: './screenshot.component.html'
})
export class ScreenshotComponent implements OnInit, OnDestroy, LokiComponent {
    component = 'screenshot';
    errors: Array<string>;
    image_name: string;
    images: Array<string>;
    screenshot_loading: boolean;

    @ViewChild('modal') modal: ModalComponent;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.image_name = undefined;
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
