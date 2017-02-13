import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By }                               from '@angular/platform-browser';
import { DebugElement }                     from '@angular/core';

import { LokiComponent }                    from '../../../interfaces/loki-component.interface';
import { SocketMessage }                    from '../../../interfaces/socket-message.interface';
import { SocketService }                    from '../../../services/socket.service';
import { ErrorDisplayComponent }            from '../../error-display/error-display.component';
import { DeviceListComponent }              from './device-list.component';

describe('DeviceListComponent', () => {

    let component:      DeviceListComponent;
    let fixture:        ComponentFixture<DeviceListComponent>;
    let debugElement:   DebugElement;
    let element:        HTMLElement;

    // a fake socket service
    let socketServiceStub: {
        register(addon: LokiComponent): void;
        unregister(addonToRemove: LokiComponent): void;
        sendMessage(message: SocketMessage): void;
    };

    // fake process information for the component
    let fakeDeviceInfo: SocketMessage = {
        component: 'device-list',
        specifier: undefined,
        action: 'getDevices',
        data: [
            {
                "className": "Image",
                "deviceInstanceId": 'Some Instance ID',
                "friendlyName": 'Some Friendly Name',
                "hardwareId": 'Some Hardware ID'
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
                    case 'getDevices':
                        // send some fake data back to the component
                        component.processMessage(fakeDeviceInfo);
                        break;
                }
            }
        }
        TestBed.configureTestingModule({
            declarations: [
                DeviceListComponent,
                ErrorDisplayComponent
            ],
            providers: [ {provide: SocketService, useValue: socketServiceStub} ]
        }).compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(DeviceListComponent);

        component = fixture.componentInstance;
    });

    it('should have no errors at startup', () => {
        expect(component.errors.length).toBe(0);
    });

    it('should have no process-list-table visible at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.device-list-table'));
        expect(debugElement).toBeNull();
    });

    it('should request process information at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.device-list-table'));
        expect(debugElement).toBeNull();
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.device-list-table'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });
});
