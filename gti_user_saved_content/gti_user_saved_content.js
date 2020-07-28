(function ($) {

  $(document).ready(function() {
    //handle the redirects for our custom blocks
    $('.gti-saved-content-count-redirect').hide();
    $('.gti-saved-content-count').one('click', function (e) {
      //make sure we only redirect if the redirect href class exists
      if($(this).find('.gti-saved-content-count-redirect').length) {
        window.location = $(this).find('.gti-saved-content-count-redirect').attr('href');
      }
    });
  });
  $(document).ajaxStop(function(){
    $('.gti-saved-content-count-redirect').hide();
    $('.gti-saved-content-count').one('click', function (e) {
      //make sure we only redirect if the redirect href class exists
      if($(this).find('.gti-saved-content-count-redirect').length) {
        window.location = $(this).find('.gti-saved-content-count-redirect').attr('href');
      }
    });
  });


  var gti_user_saved_content = {
    moduleName: "gti_user_saved_content",
    runOnce: true,
    hasRunOnce: false,
    run: function (context,settings,userData) {
      var isCached = true;
      var aOptions = {};
      var that = this;
      var processed = false;
      var requestDone = false;

      jQuery(document).ajaxComplete(function (event, xhr, eventSettings) {
        if(requestDone){
          processed = false;
        }
        if (eventSettings.url.match('search_api_ajax\/search') || eventSettings.url.match('gti-user-saved-content-save-button')) {
          if(!processed) {
            jQuery(document).ready(function () {
              setTimeout(function () {
                $('#save_search').click(function(){
                  $('.popup-element-title').addClass('saved-search');
                });
                var node = {};
                node.nid = [];
                var path = {};
                path.current = window.location.href.replace('http://'," ");;
                jQuery.each(jQuery('.dashboard'), function (k, v) {
                  node.nid.push(jQuery(v).data('node'));
                });

                node.uid = userData.uid;
                if (node.uid != 0) {
                  $.ajax({
                    type: "GET",
                    data: path,
                    url: window.location.origin + "/saved-content/saved-search",
                    async: true,
                    dataType: "json",
                    success: function (data, status, jqXHR) {
                      if(typeof(data.saved_search) != "undefinded"){
                        if(data.saved_search == 1){
                          $('.popup-element-title').addClass('saved-search').find('span').html('Saved search');
                        }
                      }


                    },
                    error: function (response) {
                      console.log("Error with request");
                    }
                  });


                  $.ajax({
                    type: "GET",
                    data: node,
                    url: window.location.origin + "/save-content/authcache",
                    async: true,
                    dataType: "json",
                    success: function (data, status, jqXHR) {
                      if (status == "success") {
                        processed = true;
                        var applyButtons = document.getElementsByClassName("ats-apply-button");
                        for (i = 0; i < applyButtons.length; i++) {
                          if(applyButtons[i].parentNode.className.indexOf("field-item") > -1) { // apply only to field-item
                            for (y = 0; y < applyButtons[i].childNodes.length; y++) {
                              var childClasses = applyButtons[i].childNodes[y].className;
                              if (childClasses.indexOf("gti-abacus-link")) {
                                var button_text = data[node.nid[0]].button_text;
                                if (button_text.length > 0) applyButtons[i].childNodes[y].innerText = button_text;
                              }
                            }
                          }
                        }

                        jQuery.each(jQuery('.dashboard'), function (k, v) {
                          var a = jQuery(v).find('a');
                          a.removeClass('replace-link');
                          if(typeof(data[jQuery(v).data('node')]) != "undefined"){


                            a.addClass(data[jQuery(v).data('node')].linktype + '-link');
                            a.parent().attr('data-linktype',data[jQuery(v).data('node')].linktype );
                            jQuery(v).data('linktype',data[jQuery(v).data('node')].linktype);
                            if(!a.hasClass('use-ajax')){
                              a.addClass('use-ajax');
                            }
                            a.attr('href',"/save-content/nojs/"+jQuery(v).data('node')+"/replace");
                            if (data[jQuery(v).data('node')].linktype == "add") {
                              a.attr('title', 'Shortlist');
                              a.text('Save to dashboard')
                            } else {
                              a.attr('title', 'Shortlisted');
                              a.text('Remove from dashboard');

                            }
                          }
                        });
                        if(typeof(Drupal.behaviors.AJAX) != "undefined") {
                          Drupal.behaviors.AJAX.attach(context, Drupal.settings);
                        }
                        that.pager(context, settings);
                        //    that.useAjax(context, settings);
                        that.searchButton(context, settings);
                        that.preventFacetLinkRedirection();
                      } else {
                        console.log("Error with request");
                      }
                    },
                    error: function (response) {
                      console.log("Error with request");
                    }
                  });
                }

              }, 300);
              processed = true;

            });
          }
          requestDone = true;
        }else if(eventSettings.url.match('search_api_saved_searches_save_form') && !eventSettings.url.match('authcache.php')){
          if(!processed) {
            jQuery(document).ready(function () {
              setTimeout(function () {
                $('#save_search').click(function(){
                  $('.popup-element-title').addClass('saved-search');
                });
                var node = {};
                node.nid = [];
                var path = {};
                path.current = window.location.href.replace('http://'," ");

                jQuery.each(jQuery('.dashboard'), function (k, v) {
                  node.nid.push(jQuery(v).data('node'));
                });

                node.uid = userData.uid;
                $.ajax({
                  type: "GET",
                  data: path,
                  url: window.location.origin + "/saved-content/saved-search",
                  async: true,
                  dataType: "json",
                  success: function (data, status, jqXHR) {
                    if(typeof(data.saved_search) != "undefinded"){
                      if(data.saved_search == 1){
                        $('.popup-element-title').addClass('saved-search').find('span').html('Saved search');
                      }
                    }
                  },
                  error: function (response) {
                    console.log("Error with request");
                  }
                });
                $.ajax({
                  type: "GET",
                  data: node,
                  url: window.location.origin + "/save-content/authcache",
                  async: true,
                  dataType: "json",
                  success: function (data, status, jqXHR) {
                    if (status == "success") {

                      var applyButtons = document.getElementsByClassName("ats-apply-button");
                      for (i = 0; i < applyButtons.length; i++) {
                        if(applyButtons[i].parentNode.className.indexOf("field-item") > -1) { // apply only to field-item
                          for (y = 0; y < applyButtons[i].childNodes.length; y++) {
                            var childClasses = applyButtons[i].childNodes[y].className;
                            if (childClasses.indexOf("gti-abacus-link")) {
                              var button_text = data[node.nid[0]].button_text;
                              if (button_text.length > 0) applyButtons[i].childNodes[y].innerText = button_text;
                            }
                          }
                        }
                      }

                      jQuery.each(jQuery('.dashboard'), function (k, v) {
                        var a = jQuery(v).find('a');
                        a.removeClass('replace-link');
                        if(typeof(data[jQuery(v).data('node')]) != "undefined"){


                          a.addClass(data[jQuery(v).data('node')].linktype + '-link');
                          a.parent().attr('data-linktype',data[jQuery(v).data('node')].linktype );
                          jQuery(v).data('linktype',data[jQuery(v).data('node')].linktype);
                          if(!a.hasClass('use-ajax')){
                            a.addClass('use-ajax');
                          }
                          a.attr('href',"/save-content/nojs/"+jQuery(v).data('node')+"/replace");
                          if (data[jQuery(v).data('node')].linktype == "add") {
                            a.attr('title', 'Shortlist');
                            a.text('Save to dashboard')
                          } else {
                            a.attr('title', 'Shortlisted');
                            a.text('Remove from dashboard');

                          }
                        }
                      });
                      if(typeof(Drupal.behaviors.AJAX) != "undefined") {
                        Drupal.behaviors.AJAX.attach(context, Drupal.settings);
                      }
                      that.pager(context, settings);
                      //    that.useAjax(context, settings);
                      that.searchButton(context, settings);
                      that.preventFacetLinkRedirection();
                    } else {
                      console.log("Error with request");
                    }
                  },
                  error: function (response) {
                    console.log("Error with request");
                  }
                });

              }, 300);

              processed = true;

            });
          }
          requestDone = true;
        }

      });
console.log();
      if(window.location.href.match('/?')){
        jQuery(document).ready(function(){
          setTimeout(function(){
            $('#save_search').click(function(){
              $('.popup-element-title').addClass('saved-search');
            });


            var node = {};
            node.nid = [];
            var path = {};
            path.current = window.location.href.replace('http://'," ");
            jQuery.each(jQuery('.dashboard'), function (k, v) {
              node.nid.push(jQuery(v).data('node'));
            });

            node.uid = userData.uid;
            if(node.uid != 0){
              $.ajax({
                type: "GET",
                data: path,
                url: window.location.origin + "/saved-content/saved-search",
                async: true,
                dataType: "json",
                success: function (data, status, jqXHR) {
                  if(typeof(data.saved_search) != "undefinded"){
                    if(data.saved_search == 1){
                      $('.popup-element-title').addClass('saved-search').find('span').html('Saved search');
                    }
                  }
                },
                error: function (response) {
                  console.log("Error with request");
                }
              });
              $.ajax({
                type: "GET",
                data: node,
                url: window.location.origin + "/save-content/authcache",
                async: true,
                dataType: "json",
                success: function (data, status, jqXHR) {
                  if (status == "success") {

                    // refresh apply button text
                    var applyButtons = document.getElementsByClassName("ats-apply-button");
                    for (i = 0; i < applyButtons.length; i++) {
                      if(applyButtons[i].parentNode.className.indexOf("field-item") > -1) { // apply only to field-item
                        for (y = 0; y < applyButtons[i].childNodes.length; y++) {
                          var childClasses = applyButtons[i].childNodes[y].className;
                          if (childClasses.indexOf("gti-abacus-link")) {
                            var button_text = data[node.nid[0]].button_text;
                            if (button_text.length > 0) applyButtons[i].childNodes[y].innerText = button_text;
                          }
                        }
                      }
                    }

                    jQuery.each(jQuery('.dashboard'), function (k, v) {
                      var a = jQuery(v).find('a');
                      a.removeClass('replace-link');
                      if(typeof(data[jQuery(v).data('node')]) != "undefined"){


                        a.addClass(data[jQuery(v).data('node')].linktype + '-link');
                        a.parent().attr('data-linktype',data[jQuery(v).data('node')].linktype );
                        jQuery(v).data('linktype',data[jQuery(v).data('node')].linktype);
                        if(!a.hasClass('use-ajax')){
                          a.addClass('use-ajax');
                        }
                        a.attr('href',"/save-content/nojs/"+jQuery(v).data('node')+"/replace");
                        if (data[jQuery(v).data('node')].linktype == "add") {
                          a.attr('title', 'Shortlist');
                          a.text('Save to dashboard')
                        } else {
                          a.attr('title', 'Shortlisted');
                          a.text('Remove from dashboard');

                        }
                      }
                    });
                    if(typeof(Drupal.behaviors.AJAX) != "undefined") {
                      Drupal.behaviors.AJAX.attach(context, Drupal.settings);
                    }
                    that.pager(context, settings);
                //    that.useAjax(context, settings);
                    that.searchButton(context, settings);
                    that.preventFacetLinkRedirection();
                  } else {
                    console.log("Error with request");
                  }
                },
                error: function (response) {
                  console.log("Error with request");
                }
              });
            }

          },1100);
          processed = true;

        });
      }

      if(typeof(Drupal.ajax) != "undefined"){
        Drupal.ajax.prototype.beforeSend = function (xmlhttprequest, options) {
          if (options.url.match('save-content')) {
            aOptions = options;
            options.url = options.url.replace(/replace/g, userData.uid);
          } else {
            aOptions = options;
          }
          return true;
        };

        Drupal.ajax.prototype.complete = function(xmlhttprequest,status) {
          var that = this;
          jQuery(document).ajaxComplete(function (event, xhr, eventSettings) {
            if (eventSettings.url.match('save-content\/ajax')) {
              setTimeout(function(){

                var a = jQuery(that.element);
                a = jQuery("#"+a.attr('id'));
                var parent = a.parent();
                var linktype = a.parent().data('linktype');
                var node = a.parent().data('node');
                a.removeClass('replace-link');
                if (parent.data('position') == "top" && parent.data('featured') == 1) {
                  var featured = jQuery('#save-content-main-' + node);
                  var featuredParent = featured.parent();
                  if (linktype == "add") {
                    a.removeClass('remove-link');
                    a.removeClass('add-link');
                    a.removeClass('replace-link');
                    a.addClass('add-link');
                    a.attr('title', 'Shortlist');
                    //parent.removeData('linktype');
                    // parent.data('linktype', "remove");
                    if (featured.length > 0) {
                      featured.removeClass('remove-link');
                      featured.removeClass('add-link');
                      featured.removeClass('replace-link');
                      featured.addClass('add-link');
                      featured.attr('title', 'Shortlisted');
                      featuredParent.removeData('linktype');
                      featuredParent.data('linktype', "add");
                    }


                  } else if (linktype == "remove") {
                    a.removeClass('remove-link');
                    a.removeClass('add-link');
                    a.removeClass('replace-link');
                    a.addClass('remove-link');
                    a.attr('title', 'Shortlist');
                    // parent.removeData('linktype');
                    // parent.data('linktype', "add");
                    if (featured.length > 0) {
                      featured.removeClass('remove-link');
                      featured.removeClass('add-link');
                      featured.removeClass('replace-link');
                      featured.addClass('remove-link');
                      featured.attr('title', 'Shortlist');
                      featuredParent.removeData('linktype');
                      featuredParent.data('linktype', "remove");
                    }


                  }


                } else if (parent.data('position') == "main" && parent.data('featured') == 1) {
                  var featured = jQuery('#save-content-top-' + node);
                  var featuredParent = featured.parent();
                  if (linktype == "add") {
                    a.removeClass('remove-link');
                    a.removeClass('add-link');
                    a.removeClass('replace-link');
                    a.addClass('add-link');
                    a.attr('title', 'Shortlisted');
                    // parent.removeData('linktype');
                    // parent.data('linktype', "remove");

                    if (featured.length > 0) {
                      featured.removeClass('remove-link');
                      featured.removeClass('add-link');
                      featured.removeClass('replace-link');
                      featured.addClass('add-link');
                      featured.attr('title', 'Shortlisted');
                      featuredParent.removeData('linktype');
                      featuredParent.data('linktype', "add");
                    }


                  } else if (linktype == "remove") {
                    a.removeClass('remove-link');
                    a.removeClass('add-link');
                    a.removeClass('replace-link');
                    a.addClass('remove-link');
                    a.attr('title', 'Shortlist');
                    // parent.removeData('linktype');
                    // parent.data('linktype', "add");
                    if (featured.length > 0) {

                      featured.removeClass('remove-link');
                      featured.removeClass('add-link');
                      featured.removeClass('replace-link');
                      featured.addClass('remove-link');
                      featured.attr('title', 'Shortlist');
                      featuredParent.removeData('linktype');
                      featuredParent.data('linktype', "remove");
                    }


                  }


                } else {
                  if (linktype == "add") {
                    a.removeClass('remove-link');
                    a.removeClass('add-link');
                    a.removeClass('replace-link');
                    a.addClass('add-link');
                    a.attr('title', 'Shortlisted');
                    //parent.removeData('linktype');
                    // parent.data('linktype', "remove");
                  } else if (linktype == "remove") {
                    a.removeClass('remove-link');
                    a.removeClass('add-link');
                    a.removeClass('replace-link');
                    a.addClass('remove-link');
                    a.attr('title', 'Shortlist');
                    // parent.removeData('linktype');
                    //  parent.data('linktype', "remove");
                  }


                }

              },500);

            }
          });
          return true;
        }

      }


    },
    pager: function (context, settings) {
      var that = this;
      jQuery('.pager-item').find('a').click(function () {

        jQuery(document).ajaxComplete(function (event, xhr, eventSettings) {

          if (eventSettings.url.match('/views/ajax')) {


            jQuery.each(jQuery('.dashboard'), function (k, v) {


              jQuery(v).find('a').removeClass('replace-link');
              jQuery(v).find('a').addClass(jQuery(v).data('linktype') + '-link');

            });

            if(typeof(Drupal.behaviors.AJAX) != "undefined") {
              Drupal.behaviors.AJAX.attach(context, Drupal.settings);
            }
            that.pager(context, Drupal.settings);
          //  that.useAjax(context, Drupal.settings);
            that.searchButton(context, Drupal.settings);
            that.preventFacetLinkRedirection();
          }

        });

      });
    },
    preventFacetLinkRedirection:function(){
      jQuery('.facetapi-facetapi-links > li').find('a').click(function(e){
        e.preventDefault(e);
      });
    },
    searchButton:function(context, settings){
      var that = this;
      jQuery('#edit-submit-search').click(function () {

        jQuery(document).ajaxComplete(function (event, xhr, eventSettings) {

          if (eventSettings.url.match('/views/ajax')) {


            jQuery.each(jQuery('.dashboard'), function (k, v) {


              jQuery(v).find('a').removeClass('replace-link');
              jQuery(v).find('a').addClass(jQuery(v).data('linktype') + '-link');

            });
            if(typeof(Drupal.behaviors.AJAX) != "undefined") {
              Drupal.behaviors.AJAX.attach(context, Drupal.settings);
            }
            that.pager(context, Drupal.settings);
           // that.useAjax(context, Drupal.settings);
            that.searchButton(context, Drupal.settings);
            that.preventFacetLinkRedirection();
          }

        });

      });

    }
  }

  var url = window.location.origin;

  if(url.match('targetjobs')){
    if(typeof(Drupal.behaviors.gti_authcache) != "undefined"){

      Drupal.behaviors.gti_authcache.attachObject(gti_user_saved_content);
    }else if(Drupal.settings.gtiAbacus.uid == 1){
      var context = "";
      var settings = "";
      var userData = {
        "uid": Drupal.settings.gtiAbacus.uid,
        "theme_token": Drupal.settings.ajaxPageState.theme_token,
        "pageviews_uuid": Drupal.settings.gtiAbacus.pageviews_uuid
      };
      gti_user_saved_content.run(context, settings, userData);
    }

  }


})(jQuery);
