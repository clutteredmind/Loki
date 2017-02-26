import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By }                               from '@angular/platform-browser';
import { DebugElement }                     from '@angular/core';

import { LokiComponent }                    from '../../../interfaces/loki-component.interface';
import { SocketMessage }                    from '../../../interfaces/socket-message.interface';
import { SocketService }                    from '../../../services/socket.service';
import { Ng2Bs3ModalModule }                from 'ng2-bs3-modal/ng2-bs3-modal';
import { ErrorDisplayComponent }            from '../../error-display/error-display.component';
import { SpinnerComponent }                 from '../../spinner/spinner.component';
import { ScreenshotComponent }              from './screenshot.component';

describe('ScreenshotComponent', () => {

    let component:      ScreenshotComponent;
    let fixture:        ComponentFixture<ScreenshotComponent>;
    let debugElement:   DebugElement;
    let element:        HTMLElement;

    // a fake socket service
    let socketServiceStub: {
        register(addon: LokiComponent): void;
        unregister(addonToRemove: LokiComponent): void;
        sendMessage(message: SocketMessage): void;
    };

    // version number to plug in below
    let fileName: string = 'some_file_name.jpg';

    // async beforeEach
    beforeEach(async(() => {
        socketServiceStub = {
            register(addon: LokiComponent): void {

            },

            unregister(addonToRemove: LokiComponent): void {

            },

            sendMessage(message: SocketMessage): void {
                switch(message.action) {
                    case 'captureScreen':
                        component.processMessage({
                            component: 'screenshot',
                            specifier: 'ScreenshotModule',
                            action: 'captureScreen',
                            data: {
                                image_name: fileName
                            },
                            parameters: undefined
                        });
                        break;
                    case 'getScreenshotList':
                        component.processMessage({
                            component: 'screenshot',
                            specifier: 'ScreenshotModule',
                            action: 'getScreenshotList',
                            data: {
                                image_list: ['the_first_file.jpg', 'the second_file.jpg', 'the_third_file.jpg']
                            },
                            parameters: undefined
                        });
                        break;
                }
            }
        }
        TestBed.configureTestingModule({
            imports: [
                Ng2Bs3ModalModule
            ],
            declarations: [
                ScreenshotComponent,
                ErrorDisplayComponent,
                SpinnerComponent
            ],
            providers: [ {provide: SocketService, useValue: socketServiceStub} ]
        }).compileComponents();
    }));

    // synchronous beforeEach
    beforeEach(() => {
        fixture = TestBed.createComponent(ScreenshotComponent);

        component = fixture.componentInstance;
    });

    it('should have no errors at startup', () => {
        expect(component.errors.length).toBe(0);
    });

    it('should not display the screenshot area at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.screenshot-panel-body'));
        expect(debugElement).toBeNull();
    });

    it('should not display the screenshot list panel at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.screenshot-list-panel'));
        expect(debugElement).toBeNull();
    });

    it('should request a list of screenshots at startup', () => {
        debugElement = fixture.debugElement.query(By.css('div.screenshot-list-panel'));
        expect(debugElement).toBeNull();
        fixture.detectChanges();
        debugElement = fixture.debugElement.query(By.css('div.screenshot-list-panel'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });

    it('should display a screenshot when a filename returns after clicking the screenshot button', () => {
        debugElement = fixture.debugElement.query(By.css('div.screenshot-panel-body'));
        expect(debugElement).toBeNull();
        // click get screenshot button
        debugElement = fixture.debugElement.query(By.css('a.get-screenshot-button'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
        element.click();
        // process the change
        fixture.detectChanges();
        // check to see if the screenshot area is being displayed now
        debugElement = fixture.debugElement.query(By.css('div.screenshot-panel-body'));
        element = debugElement.nativeElement;
        expect(element).not.toBeNull();
    });
});
