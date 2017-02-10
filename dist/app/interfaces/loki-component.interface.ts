import { SocketMessage } from './socket-message.interface';

export interface LokiComponent {
    component: string;
    errors: Array<string>;
    processMessage(message: SocketMessage): void;
}
