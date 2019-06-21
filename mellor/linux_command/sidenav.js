var max_height = document.documentElement.clientHeight;
var jumbotron_height = document.getElementById("jumbotron-header").clientHeight;
var footer_position = document.getElementById("footer").offsetTop;
var sidenav = document.getElementById("sidenav");
var window_height = window.innerHeight;
var bottom_of_sidenav = window_height - footer_position;
var scale_vh = 100 / max_height;
var need_to_resize_sidenav = true;
function debounce(f, t) {
	return function (args) {
		let previousCall = this.lastCall;
		this.lastCall = Date.now();
		if (previousCall && ((this.lastCall - previousCall) <= t)) {
			clearTimeout(this.lastCallTimer);
		}
		this.lastCallTimer = setTimeout(() => f(args), t);
	}
}

window.onscroll = sidenav_height_adj;

function sidenav_height_adj() {
	var scroll_location = document.documentElement.scrollTop;
	if (scroll_location < jumbotron_height) {
		sidenav.style.top = jumbotron_height - scroll_location + "px";
		need_to_resize_sidenav = true;
	} else {
		var amount_of_visible_footer = scroll_location + bottom_of_sidenav;
		if (amount_of_visible_footer > 0) {
			sidenav.style.bottom = amount_of_visible_footer + "px";
			need_to_resize_sidenav = true;
		} else {
			if (need_to_resize_sidenav) {
				sidenav.style.top = "0px";
				sidenav.style.bottom = "0px";
				need_to_resize_sidenav = false;
			}
		}
	}
}
window.onresize = debounce(change_max_height, 50);

function change_max_height() {
	window_height = window.innerHeight;
	bottom_of_sidenav = window_height - footer_position;
	max_height = document.documentElement.clientHeight;
	scale_vh = 100 / max_height;
	sidenav_height_adj();
}
