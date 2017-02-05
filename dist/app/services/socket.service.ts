import { Injectable, OnDestroy }    from '@angular/core';

import { Addon }                    from '../interfaces/addon.interface';
import { SocketMessage }            from '../interfaces/socket-message.interface';

@Injectable()
export class SocketService implements OnDestroy {
    socket: WebSocket;
    addons: Array<Addon>;

    constructor() {
        this.addons = new Array<Addon>();
        // connect to the server
        var host_string = location.host.indexOf(':') ? location.host.substr(0, location.host.indexOf(':')) : location.host ;
        this.connectToServer('ws://' + host_string + ':8889');
    }

    register(addon: Addon): void {
        var index = this.addons.push(addon);
        this.socket.send(JSON.stringify({
            category: 'system',
            action: 'register',
            data: addon.category
        }));
    }

    unregister(addonToRemove: Addon): void {
        this.socket.send(JSON.stringify({
            category: 'system',
            action: 'unregister',
            data: addonToRemove.category
        }));
        this.addons.forEach((addon, index) => {
            if(addonToRemove == addon) {
                this.addons.splice(index, 1);
            }
        });
    }

    sendMessage(message: SocketMessage): void {
        this.socket.send(JSON.stringify(message));
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
                    if(addon.category == data.category) {
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
