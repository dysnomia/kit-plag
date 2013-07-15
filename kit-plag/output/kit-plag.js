(function() {
    /*jshint validthis: true */
    "use strict";

    // Currently selected measure and query/reference pair
    var current_measure, current_query;

    // Equal test with epsilon
    function isEqual(x, y) {
        return Math.abs(x - y) < 1e-6;
    }

    // Get selector for (measure, query) combination
    function getTextSelector() {
        return current_measure + " > " + current_query;
    }

    // Get hash-class of obj (first class beginning with id-)
    function getMatchClass(obj) {
        return "." + $(obj).attr("class").split(" ").filter(function(str) {
            return str.slice(0, 3) === "id-";
        })[0];
    }

    // Select new measure or text
    function selectMeasure() {
        $(getTextSelector()).hide();
        $(current_measure).hide();
        current_measure = $(this).val();
        $(current_measure).show();
        $(getTextSelector()).show();
    }
    function selectText() {
        $(getTextSelector()).hide();
        current_query = $(this).val();
        $(getTextSelector()).show();
    }

    // Toggle highlighting of matching hash-pairs
    function toggleHighlightClick() {
        $(getMatchClass(this)).toggleClass("highlighted-click");
        $(getMatchClass(this)).find('span').toggleClass("highlighted-click");
    }
    function toggleHighlightHover() {
        $(getMatchClass(this)).toggleClass("highlighted-hover");
        $(getMatchClass(this)).find('span').toggleClass("highlighted-hover");
    }

    // Create the complete summary table
    function createSummary() {
        var output = '<table class="global-summary"><thead>', $output,
            measure_names = $("#select-measure option").toArray();

        // Basic table structure
        output += $('.local-summary thead tr').html();
        output += "</thead><tbody>";
        $(".local-summary tbody").each(function() {
            output += $(this).html();
        });
        output += "</tbody></table>";

        // Add first column with measure names
        $output = $(output);
        $output.find("thead tr").prepend("<th></th>");
        $output.find("tbody tr").each(function(index) {
            $(this).prepend('<th>' + $(measure_names[index]).text() + "</th>");
        });

        return $output;
    }

    // Sets minimum width to at least query text + reference text + offset
    // as well as the query and reference pre's to their respective maximum width
    function setMinWidth() {
        var max_query     = 0, /* Maximum width of query */
            max_reference = 0; /* Maximum width of reference */

        // Compute minimum width (enable elements so their width can be calculated)
        $('.measure').each(function() {
            $(this).show();
            $(this).find('.compare-container').each(function() {
                $(this).show();

                $(this).find('.query pre').each(function() {
                    max_query = Math.max(max_query, $(this).width());
                });
                $(this).find('.ref pre').each(function() {
                    max_reference = Math.max(max_reference, $(this).width());
                });

                $(this).hide();
            });
            $(this).hide();
        });

        // Set widths
        $('body').css('min-width', ((max_reference + max_query) * 1.0/0.90) + "px");
        $('.query pre').each(function() {
            $(this).width(max_query);
        });
        $('.ref pre').each(function() {
            $(this).width(max_reference);
        });
    }

    // Color table cells containing numbers by their relative value
    function colorTableCells() {
        $('tr').each(function() {
            // Get maximum value in this row
            var factor = 0.0;
            $(this).find('td').each(function() {
                var val = parseFloat($(this).text());
                if (!isNaN(val) && !isEqual(val, 1.0)) {
                    factor = Math.max(factor, val);
                }
            });

            // Color cells according to this maximum
            $(this).find('td').each(function() {
                var val = parseFloat($(this).text()), color;
                if (!isNaN(val)) {
                    val = Math.min(val / factor, 1.0) * 100;
                    color = "rgb(*%,-%,0%)".replace(/\*/g, val).replace(/-/g, 100 - val);
                    $(this).css('color', color);
                }
            });
        });
    }

    // Mark table cells representing reference texts that should be recognized as plagiarisms.
    function markPlagiarism() {
        $('th.plagiarism').append('*');
    }

    // Main
    $(function() {
        /* Hack: fix weird timing problem in chrome that I cannot really identify */
        window.setTimeout(function() {
            setMinWidth();
            $('#select-measure').change(selectMeasure).trigger('change');
            $('#select-query').change(selectText).trigger('change');
            $('.marked').click(toggleHighlightClick).hover(toggleHighlightHover, toggleHighlightHover);
            $('#global-summary').after(createSummary());
            colorTableCells();
            markPlagiarism();
        }, 0);
    });
})();
