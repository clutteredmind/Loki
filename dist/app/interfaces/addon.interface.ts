import { SocketMessage } from './socket-message.interface';

export interface Addon {
    component: string;
    errors: Array<string>;
    processMessage(message: SocketMessage): void;
}
