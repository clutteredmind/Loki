export interface Process {
    processName: string;
    processProcessId: number;
    processThreadCount: number;
    processParentProcessId: number;
    processPriorityBase: number;
}
