/*
  This file is part of the "FnordMetric" project
    Copyright (c) 2014 Laura Schlimmer
    Copyright (c) 2014 Paul Asmuth, Google Inc.
  FnordMetric is free software: you can redistribute it and/or modify it under
  the terms of the GNU General Public License v3.0. You should have received a
  copy of the GNU General Public License along with this program. If not, see
  <http://www.gnu.org/licenses/>
*/

var TabbableComponent = function() {
  this.createdCallback = function() {
    this.init();
  };
  this.attributeChangedCallback = function(attr, old_val, new_val) {
    if (attr == 'data-content-attached') {
      this.init();
    }
  }
  this.init = function() {
    var active_tab = this.querySelector("z-tab[data-active]");
    if (active_tab == null) {
      var tabs = this.querySelectorAll("z-tab");
      if (tabs && tabs[0]) {
        tabs[0].setAttribute('data-active', 'active');
        active_tab = tabs[0];
      }
    }
    if (this.hasAttribute('data-content-attached')) {
      var index = active_tab.getAttribute('data-index');
      this.setContent(index);
    }
    if (this.hasAttribute('data-evenly-sized')) {
      this.setTabWidth();
    }
    this.observeTabs();
  }
  this.setContent = function(index) {
    var active_content = this.querySelector("z-tab-content[data-active]");
    if (active_content) {
      active_content.removeAttribute('data-active');
    }
    var tab_content =
      this.querySelector("z-tab-content[data-index='" + index + "']");
    if (tab_content) {
      tab_content.setAttribute('data-active', 'active');
    }
  };
  this.setTabWidth = function() {
    var tabs = this.querySelectorAll("z-tab");
    if (tabs.length == 0) {
      return;
    }
    var width = 100 / tabs.length;
    for (var i = 0; i < tabs.length; i++) {
      tabs[i].style.width = width + "%";
    }
  }
  this.observeTabs = function() {
    var base = this;
    var tabs = this.querySelectorAll("z-tab");
    for (var i = 0; i < tabs.length; i++) {
      tabs[i].addEventListener('click', function() {
        var current_tab = base.querySelector("z-tab[data-active]");
        current_tab.removeAttribute("data-active");
        this.setAttribute("data-active", "active");
        if (base.hasAttribute('data-content-attached')) {
          var index = this.getAttribute('data-index');
          if (index) {
            base.setContent(index);
          }
        }
        base.fireClickTabEvent(this);
      }, false);
    }
  }
  this.fireClickTabEvent = function(elem) {
    var ev = new CustomEvent('z-tabbable-click', {
      bubbles: true,
      cancelable: true});
    elem.dispatchEvent(ev);
  };
};

var proto = Object.create(HTMLElement.prototype);
TabbableComponent.apply(proto);
document.registerElement("z-tabbable", { prototype: proto });
