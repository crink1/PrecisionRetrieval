function Search(){
 
    let query = $(".container .search input").val();
    console.log("query = " + query); 

  
    $.ajax({
        type: "GET",
        url: "/s?word=" + query,
        success: function(data){
            console.log(data);
            BuildHtml(data);
        }
    });
}

function BuildHtml(data){
    
    let result_lable = $(".container .result");
    result_lable.empty();

    for( let elem of data){
        let a_lable = $("<a>", {
            text: elem.title,
            href: elem.url,
            target: "_blank"
        });
        let p_lable = $("<p>", {
            text: elem.desc
        });
        let i_lable = $("<i>", {
            text: elem.url
        });
        let div_lable = $("<div>", {
            class: "item"
        });
        a_lable.appendTo(div_lable);
        p_lable.appendTo(div_lable);
        i_lable.appendTo(div_lable);
        div_lable.appendTo(result_lable);
    }
}