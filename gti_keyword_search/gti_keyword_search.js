(function ($) {

  function param(name){
    return (window.location.href.split(name+'=')[1] || "").split('&')[0];
  }
  /*
     Sets the options for the sort by for the quick filers.
   */
  function setSortOptionsList(){
    $('#edit-sort-by option').each(function(){

      if($(this).val() == "field_publish_date" || $(this).val() == "field_ad_vac_closing_date" || $(this).val() == "field_organisation_name_field_sortable_organisation_name" || $(this).val() == "field_organisation_name_field_sortable_organisation_name_1"){

        $(this).hide();
      }
    });
    $('#edit-sort-by').val('search_api_relevance');
  }
  /*
     CSS for the search filters.
   */
  function setVacanciesCss(){
    $('body').removeClass('group-facet-advice');
    $('body').removeClass('group-facet-employers');
    if(!$('body').hasClass('group-facet-vacancies')){
      $('body').addClass('group-facet-vacancies');
    }
  }
  /*
    Turns the search filters off
   */
  function setAdviceCss(){
    $('body').removeClass('group-facet-vacancies');
    $('body').removeClass('group-facet-employers');
    if(!$('body').hasClass('group-facet-advice')){
      $('body').addClass('group-facet-advice');
    }
  }
  /*
    Turns the search filters off
   */
  function setEmployersCss(){
    $('body').removeClass('group-facet-vacancies');
    $('body').removeClass('group-facet-advice');
    if(!$('body').hasClass('group-facet-employers')){
      $('body').addClass('group-facet-employers');
    }
  }
  /*
    Turns the search filters off
   */
  function setVideoCss(){
    $('body').removeClass('group-facet-vacancies');
    $('body').removeClass('group-facet-advice');
    if(!$('body').hasClass('group-facet-employers')){
      $('body').addClass('group-facet-employers');
    }
  }

  var keywordDone = false;
  /*
     resets the variables for the search filters when selected and unselecting.
   */
  function setSearchFilterClicks(){
    $('.facetapi-facetapi-links').find(".facetapi-inactive").click(function(e){

      keywordDone = false;
      e.stopPropagation();
      var url = $(e.target).attr('href');
      window.location.href = url;
    });
    $('.facetapi-facetapi-links').find(".facetapi-active").click(function(e){

      keywordDone = false;

      e.stopPropagation();
      var url = $(e.target).attr('href');
      window.location.href = url;
    });
    $('.chosen-choices').find('input').focus(function(){
      setTimeout(function(){
        $('.chosen-results > li').click(function(){

          keywordDone = false;

        });
      },1000);

    });

    $('.search-choice-close').click(function(){

      keywordDone = false;
    });

  }

  Drupal.behaviors.gti_keyword_search = {
    attach: function(context, settings) {
      $(document).ready(function(){

        var url = window.location.href;
        // Keyword search input field
        $('#edit-query').focus(function(){
          keywordDone = false;
          $('#edit-sort-by').val('search_api_relevance');
        });
        // Sort by Drop down list
        $('#edit-sort-by').click(function(e){
          keywordDone = false;
        });
        // Change results set on list change
        $('#edit-sort-by').change(function(e){
          $('#edit-submit-search').trigger('click');
        });
        // Degree subjects list
        $('.chosen-results > li').click(function(e){
          keywordDone = false;
        });
        // Organisaions hiring list
        $('.chosen-drop > li').click(function(e){
          keywordDone = false;
        });
        // Remove the last class as this is for the clear filters.
        if($('#facetapi-facet-search-apidefault-node-index-block-group-facet > li').length >= 1){

          $('#facetapi-facet-search-apidefault-node-index-block-group-facet > li.last').removeClass('last');
        }
        // Add body classes and change sort list per filter option
        if($('#facetapi-facet-search-apidefault-node-index-block-group-facet > li > a').hasClass('facetapi-active')){

          if(url.match('Vacancies')){
            setVacanciesCss();
            $('#edit-sort-by').val('field_ad_vac_closing_date');
            if($(".search-subhead").length > 0){
              setSearchFilterClicks();
            }
          }else if(url.match("Advice")){
            setAdviceCss();
            setSortOptionsList();
          }else if(url.match("Employers")){
            setEmployersCss();
            setSortOptionsList();
          }else if(url.match("Video")){
            setVideoCss();
            setSortOptionsList();
          }else{
            $('body').removeClass('group-facet-vacancies');
            $('body').removeClass('group-facet-advice');
            $('body').removeClass('group-facet-employers');
          }

          // With one of the quick filters active add the clear filter button
          if($('#clear-filter').length == 0){
            $('#facetapi-facet-search-apidefault-node-index-block-group-facet').append("<li class=\"leaf lastfilter\"><a id='clear-filter' href='#' rel=\"nofollow\" class=\"facetapi-inactive\" >Clear filters</a></li>");
          }else{
            $('#clear-filter').parent().addClass('lastfilter');
          }
          // Remove the active quick filter
          $('#clear-filter').click(function(e){
            e.stopPropagation();
            var url = $('.facetapi-active').attr('href');

            window.location.href = url;
          });
          // Set the search filter settings
          if($(".search-subhead").length > 0){
            setSearchFilterClicks();
          }
        }

        // reset the keyword tick code if one of the classes is clicked
        $('.facetapi-inactive,.facetapi-active,.first,.active,.last').click(function(e){
          keywordDone = false;
        });
        // reset the keyword tick code and redirect to quick filter
        $('#facetapi-facet-search-apidefault-node-index-block-group-facet > li > a').click(function(e){
          keywordDone = false;
          var url = $(e.target).attr('href');
          window.location.href = url;
        });



          // Added set time out as in some cases the tick is not being added
         // setTimeout(function(){
            // If keyword exists add the tick
            if($('#edit-query').val() != ""){
              if(!keywordDone) {
                var url = window.location.href;
                if (url.match('&')) {
                  url = url.replace('query=' + $('#edit-query').val() + "&", "query=&");
                  if(url.match('#query')){
                    url = url.replace('#query=' + $('#edit-query').val() + "&", "#query=&");
                  }
                } else {
                  url = url.replace('query=' + $('#edit-query').val(), "query=");
                  if(url.match('#query')){
                    url = url.replace('#query=' + $('#edit-query').val() , "#query=");
                  }
                }

                var html = $('.current-search-item-filters > .item-list > ul > .first').html();
                var newHtml = '<a id="search-filter-term" href="' + url + '" rel="nofollow" class=""><span class="element-invisible"> Remove Search filter </span>' + html + '</a>';
                $('.current-search-item-filters > .item-list > ul > .first').html(newHtml);
                $('#search-filter-term').click(function (e) {
                  $('#edit-query').val("");

                  keywordDone = false;

                });
                keywordDone = true;
              }
            }
        //  },1000);


        // reset all filters and keyword searches
        $('.current-search-item-clear-all').click(function(e){

          keywordDone = false;
          $('body').removeClass('group-facet-vacancies');
        });
        // Current search reset
        $('.current-search-item-filters > div > ul > li > a').click(function(e){

          keywordDone = false;
          if($(e.target).html().match('Vacancies')){
            $('body').removeClass('group-facet-vacancies');
          }
        });
      });

      jQuery(document).ajaxComplete(function (event, xhr, eventSettings) {

        if (eventSettings.url.match('group_facet') || eventSettings.url.match('/search/all')) {
console.log("testing");
          // Keyword search input field
          $('#edit-sort-by').click(function(e){
            keywordDone = false;
          });
          // Sort by Drop down list
          $('#edit-query').focus(function(){

            keywordDone = false;
            $('#edit-sort-by').val('search_api_relevance');
          });
          // Change results set on list change
          $('#edit-sort-by').change(function(e){
            $('#edit-submit-search').trigger('click');
          });
          // Organisaions hiring list
          $('.chosen-drop > li').click(function(e){
            keywordDone = false;
          });
          // Remove the last class as this is for the clear filters.
          if($('#facetapi-facet-search-apidefault-node-index-block-group-facet > li').length >= 1){

            $('#facetapi-facet-search-apidefault-node-index-block-group-facet > li.last').removeClass('last');
          }
          // With one of the quick filters active add the clear filter button
          if($('#clear-filter').length == 0){
            $('#facetapi-facet-search-apidefault-node-index-block-group-facet').append("<li class=\"leaf lastfilter\"><a id='clear-filter' href='#' rel=\"nofollow\" class=\"facetapi-inactive\" >Clear filters</a></li>");
          }else{
            $('#clear-filter').parent().addClass('lastfilter');
          }
          // Remove the active quick filter
          $('#clear-filter').click(function(e){

            keywordDone = false;
            e.stopPropagation();
            var url = $('.facetapi-active').attr('href');
            window.location.href = url;
          });
          // Set the search filter settings
          if($(".search-subhead").length > 0){
            setSearchFilterClicks();
          }
          // reset the keyword tick code and redirect to quick filter
          $('#facetapi-facet-search-apidefault-node-index-block-group-facet > li > a').click(function(e){
            keywordDone = false;
            var url = $(e.target).attr('href');
            window.location.href = url;
          });
          // Add body classes and change sort list per filter option
          if($('#facetapi-facet-search-apidefault-node-index-block-group-facet > li > a').hasClass('facetapi-active')){
            if(eventSettings.url.match('Vacancies')){
              setVacanciesCss();
              $('#edit-sort-by').val('field_ad_vac_closing_date');
            }

            if(eventSettings.url.match('Advice')){
              setAdviceCss();
              setSortOptionsList();

            }

            if(eventSettings.url.match('Employers')){
              setEmployersCss();
              setSortOptionsList();
            }

            if(eventSettings.url.match("Video")){
              setVideoCss();
              setSortOptionsList();
            }
          }
          // Added set time out as in some cases the tick is not being added
        //  setTimeout(function(){
            // If keyword exists add the tick
            if($('#edit-query').val() != ""){
              if(!keywordDone){
                var url = window.location.href;
                if (url.match('&')) {
                  url = url.replace('query=' + $('#edit-query').val() + "&", "query=&");
                  if(url.match('#query')){
                    url = url.replace('#query=' + $('#edit-query').val() + "&", "#query=&");
                  }
                } else {
                  url = url.replace('query=' + $('#edit-query').val(), "query=");
                  if(url.match('#query')){
                    url = url.replace('#query=' + $('#edit-query').val() , "#query=");
                  }
                }
                var html = $('.current-search-item-filters > .item-list > ul > .first').html();
                var newHtml = '<a id="search-filter-term" href="'+url+'" rel="nofollow" class=""><span class="element-invisible"> Remove Search filter </span>'+html+'</a>';
                $('.current-search-item-filters > .item-list > ul > .first').html(newHtml);
                $('#search-filter-term').click(function(e){
                  $('#edit-query').val("");
                  keywordDone = false;

                });
                keywordDone = true;
              }
            }
         // },1000);

        }
      });
    }
  }
})(jQuery);
