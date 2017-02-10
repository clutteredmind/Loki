export interface SocketMessage {
    component: string;
    specifier: string;
    action: string;
    data: any;
    parameters?: [any];
}
