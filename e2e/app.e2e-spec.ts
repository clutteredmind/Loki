import { LokiPage } from './app.po';

describe('loki App', function() {
  let page: LokiPage;

  beforeEach(() => {
    page = new LokiPage();
  });

  it('should display message saying app works', () => {
    page.navigateTo();
    expect(page.getParagraphText()).toEqual('app works!');
  });
});
