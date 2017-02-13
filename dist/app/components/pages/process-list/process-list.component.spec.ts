import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By }                               from '@angular/platform-browser';
import { DebugElement }                     from '@angular/core';

import { LokiComponent }                    from '../../../interfaces/loki-component.interface';
import { SocketMessage }                    from '../../../interfaces/socket-message.interface';
import { SocketService }                    from '../../../services/socket.service';
import { ErrorDisplayComponent }            from '../../error-display/error-display.component';
import { ProcessListComponent }             from './process-list.component';

describe('ProcessListComponent', () => {

    let component:      ProcessListComponent;
    let fixture:        ComponentFixture<ProcessListComponent>;
    let debugElement:   DebugElement;
    let element:        HTMLElement;

    // a fake socket service
    let socketServiceStub: {
        register(addon: LokiComponent): void;
        unregister(addonToRemove: LokiComponent): void;
        sendMessage(message: SocketMessage): void;
    };

    // fake process information for the component
    let fakeProcessInfo: SocketMessage = {
        component: 'process-list',
        specifier: undefined,
        action: 'getProcesses',
        data: [
            {
                "processName": "[System Process]",
                "processProcessId": 0,
                "processThreadCount": 8,
                "processParentProcessId": 0,
                "processPriorityBase": 0
            },
            {
                "processName": "System",
                "processProcessId": 4,
                "processThreadCount": 160,
                "processParentProcessId": 0,
                "processPriorityBase": 8
            },
            {
                "processName": "smss.exe",
                "processProcessId": 400,
                "processThreadCount": 2,
                "processParentProcessId": 4,
                "processPriorityBase": 11
            },
            {
                "processName": "csrss.exe",
                "processProcessId": 592,
                "processThreadCount": 11,
                "processParentProcessId": 524,
                "processPriorityBase": 13
            },
            {
                "processName": "wininit.exe",
                "processProcessId": 712,
                "processThreadCount": 1,
                "processParentProcessId": 524,
                "processPriorityBase": 13
            }
        ],
        parameters: undefined
    }

    // async beforeEach
    beforeEach(async(() => {
        socketServiceStub = {
            register(addon: LokiComponent): void {

            },

            unregister(addonToRemove: LokiComponent): void {

            },

            sendMessage(message: SocketMessage): void {
                switch(message.action) {
                    case 'getProcesses':
                        // send some fake data back to the component
                        component.processMessage(fakeProcessInfo);
                        break;
                }
            }
        }
        TestBed.configureTestingModule({
            declarations: [
                ProcessListComponent,
                ErrorDisplayComponent
            ],
            providers: [ {provide: SocketService, useValue: socketServiceStub} ]
        }).compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(ProcessListComponent);

        component = fixture.componentInstance;
    });

    it('should have no errors at startup', () => {
        expect(component.errors.length).toBe(0);
    });

    it('should have no process-list-table visible at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.process-list-table'));
        expect(debugElement).toBeNull();
    });

    it('should request process information at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.process-list-table'));
        expect(debugElement).toBeNull();
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.process-list-table'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });
});
