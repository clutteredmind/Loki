import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By }                               from '@angular/platform-browser';
import { DebugElement }                     from '@angular/core';

import { LokiComponent }                    from '../../../interfaces/loki-component.interface';
import { SocketMessage }                    from '../../../interfaces/socket-message.interface';
import { SocketService }                    from '../../../services/socket.service';
import { ErrorDisplayComponent }            from '../../error-display/error-display.component';
import { SystemInformationComponent }       from './system-information.component';

describe('SystemInformationComponent', () => {

    let component:      SystemInformationComponent;
    let fixture:        ComponentFixture<SystemInformationComponent>;
    let debugElement:   DebugElement;
    let element:        HTMLElement;

    // a fake socket service
    let socketServiceStub: {
        register(addon: LokiComponent): void;
        unregister(addonToRemove: LokiComponent): void;
        sendMessage(message: SocketMessage): void;
    };

    // fake memory information for the component
    let fakeMemoryInfo: SocketMessage = {
        component: 'system-information',
        specifier: undefined,
        action: 'getMemoryInformation',
        data: {
            totalPhysicalMemory: 1,
            freePhysicalMemory: 2,
            totalPageFileSize: 3,
            freePageFileMemory: 4,
            availableExtendedMemory: 5,
            memoryLoad: 6
        },
        parameters: undefined
    }

    // fake CPU information for the component
    let fakeCpuInfo: SocketMessage = {
        component: 'system-information',
        specifier: undefined,
        action: 'getCpuInformation',
        data: {
            numberOfProcessors: 1,
            pageSize: 2,
            processorArchitecture: 3
        },
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
                    case 'getCpuInformation':
                        // send some fake data back to the component
                        component.processMessage(fakeCpuInfo);
                        break;
                    case 'getMemoryInformation':
                        // send some fake data back to the component
                        component.processMessage(fakeMemoryInfo);
                        break;
                }
            }
        }
        TestBed.configureTestingModule({
            declarations: [
                SystemInformationComponent,
                ErrorDisplayComponent
            ],
            providers: [ {provide: SocketService, useValue: socketServiceStub} ]
        }).compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(SystemInformationComponent);

        component = fixture.componentInstance;
    });

    it('should have no errors at startup', () => {
        expect(component.errors.length).toBe(0);
    });

    it('should have no list-group-items visible at startup', () => {
        // try to find list group items in the page's HTML
        debugElement = fixture.debugElement.query(By.css('li.list-group-item'));
        expect(debugElement).toBeNull();
    });

    it('should show memory information when memory information is present', () => {
        debugElement = fixture.debugElement.query(By.css('div.memory-info-panel-body'));
        expect(debugElement).toBeNull();
        component.memoryInfo = {
            totalPhysicalMemory: 9,
            freePhysicalMemory: 9,
            totalPageFileSize: 9,
            freePageFileMemory: 9,
            availableExtendedMemory: 9,
            memoryLoad: 9
        };
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.memory-info-panel-body'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });

    it('should show CPU information when CPU information is present', () => {
        debugElement = fixture.debugElement.query(By.css('div.cpu-info-panel-body'));
        expect(debugElement).toBeNull();
        component.cpuInfo = {
            numberOfProcessors: 9,
            pageSize: 9,
            processorArchitecture: 9
        };
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.cpu-info-panel-body'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });

    it('should get memory info from the socket service at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.memory-info-panel-body'));
        expect(debugElement).toBeNull();
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.memory-info-panel-body'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });

    it('should get CPU info from the socket service at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.cpu-info-panel-body'));
        expect(debugElement).toBeNull();
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.cpu-info-panel-body'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });
});
