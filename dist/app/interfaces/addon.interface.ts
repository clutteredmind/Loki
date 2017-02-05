import { SocketMessage } from './socket-message.interface';

export interface Addon {
    category: string;
    errors: Array<string>;
    processMessage(message: SocketMessage): void;
}
