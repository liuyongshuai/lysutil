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
});