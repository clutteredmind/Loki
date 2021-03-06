import { Component, OnInit, OnDestroy } from '@angular/core';

import { SocketService } from '../../../services/socket.service';
import { LokiComponent } from '../../../interfaces/loki-component.interface';
import { SocketMessage } from '../../../interfaces/socket-message.interface';

@Component({
    selector: 'app-home',
    templateUrl: './home.component.html',
    styleUrls: ['./home.component.css']
})
export class HomeComponent implements OnInit, OnDestroy, LokiComponent {

    component: 'home';
    errors: Array<string>;
    version: string;

    constructor(private socketService: SocketService) {
        this.errors = new Array<string>();
        this.version = "";
    }

    processMessage(message: SocketMessage): void {
        switch (message.action) {
            case 'getVersionNumber':
                this.version = message.data;
                break;
            case 'error':
                this.errors.push(message.data);
                break;
        }
    }

    getVersionNumber(): void {
        this.socketService.sendMessage({
            component: this.component,
            specifier: undefined,
            action: 'getVersionNumber',
            data: undefined
        });
    }

    ngOnInit() {
        this.socketService.register(this);
        this.getVersionNumber();
    }

    ngOnDestroy() {
        this.socketService.unregister(this);
    }
}
