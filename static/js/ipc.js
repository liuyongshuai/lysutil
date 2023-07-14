$(function () {
    $("#connect_node_btn").popover({
        html: true,
        placement: "right",
        title: "选择菜单图标",
        container: "body",
        content: function () {
            var icons = "<div class='icon-picker'>";
            $.each(bootstrapIconList, function (index, val) {
                icons += '<span class="glyphicon ' + val + '"></span>';
            });
            icons += "</div>";
            return icons;
        }
    }).on('shown.bs.popover', function () {
        $("div.icon-picker > span").tooltip({
            html: true,
            placement: "auto",
            title: function () {
                var cs = "enlarge";
                var cls = $(this).attr("class");
                var t = "<span class='" + cls + " " + cs + "'></span>";
                return t;
            }
        });
    });

    /**
     * 左侧的topic前的加号点击
     */
    $("#sidebar_menu_list").delegate("li[data-attr=\"topic\"] > a","click",function () {
        var topic_name = $(this).parent().attr("data-value");
        $("#selected_topic_list").append("<div class=\"panel panel-info panel-topic\" topic=\""+topic_name+"\"><div class=\"panel-heading\">"+topic_name+"<span class=\"glyphicon glyphicon-edit\"></span><span class=\"glyphicon glyphicon-remove\"></span></div></div>");
        $(this).parent().hide();
    });

    /**
     * 清理topic
     */
    $("#selected_topic_list").delegate("div[topic] > div.panel-heading > span.glyphicon-remove","click",function () {
        var parentObj = $(this).parent().parent();
        var topic_name = parentObj.attr("topic");
        $("#sidebar_topic_"+topic_name).show();
        parentObj.remove();
    });
});