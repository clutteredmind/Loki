import { Injectable, OnDestroy }    from '@angular/core';

import { Addon }                    from '../interfaces/addon.interface';
import { SocketMessage }            from '../interfaces/socket-message.interface';

@Injectable()
export class SocketService implements OnDestroy {
    socket: WebSocket;
    addons: Array<Addon>;
    pending_messages: Array<SocketMessage>;

    constructor() {
        this.addons = new Array<Addon>();
        this.pending_messages = new Array<SocketMessage>();

        // connect to the server
        var host_string = location.host.indexOf(':') ? location.host.substr(0, location.host.indexOf(':')) : location.host ;
        this.connectToServer('ws://' + host_string + ':8889');
    }

    register(addon: Addon): void {
        var index = this.addons.push(addon);
        this.sendMessage({
            component: 'system',
            specifier: undefined,
            action: 'register',
            data: addon.component
        });
    }

    unregister(addonToRemove: Addon): void {
        this.sendMessage({
            component: 'system',
            specifier: undefined,
            action: 'unregister',
            data: addonToRemove.component
        });
        this.addons.forEach((addon, index) => {
            if(addonToRemove == addon) {
                this.addons.splice(index, 1);
            }
        });
    }

    sendMessage(message: SocketMessage): void {
        var self = this;
        if(this.socket.readyState != WebSocket.OPEN) {
            // enqueue message so it can be sent later, when the socket is open
            this.pending_messages.push(message);
            // set a timer to clear the message queue in half a second
            setTimeout(() => { self.clearPendingMessageQueue(); }, 500);
        } else {
            this.socket.send(JSON.stringify(message));
        }
    }

    clearPendingMessageQueue() {
        // only clear the queue if there are messages to send and the socket is ready
        if(this.pending_messages.length > 0 && this.socket.readyState == WebSocket.OPEN) {
            this.pending_messages.forEach((message) => {
                this.socket.send(JSON.stringify(message));
            });
        }
    }

    // close socket when the service is destroyed
    ngOnDestroy(): void {
        this.socket.close();
    }

    connectToServer(serverAddress: string): void {
        var self = this;
        // only attempt to connect on startup or when the connection has failed
        if(self.socket == undefined || self.socket.readyState == WebSocket.CLOSED || self.socket.readyState == WebSocket.CLOSING) {
            // clear out old socket
            self.socket = null;
            self.socket = new WebSocket(serverAddress);
            self.socket.addEventListener('message', (event) => {
                // get event data
                var data = JSON.parse(event.data);
                // look for a handler for the message
                self.addons.forEach((addon) => {
                    if(addon.component == data.component) {
                        addon.processMessage(data);
                    }
                });
            });
            self.socket.onclose = (event) => {
                setTimeout(function(){self.connectToServer(serverAddress)}, 5000);
            };
        }
    }
}
