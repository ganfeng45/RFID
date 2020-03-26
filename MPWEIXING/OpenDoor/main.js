import Vue from 'vue'
import App from './App'

Vue.config.productionTip = false

// 注册全局组件
import MescrollBody from "@/components/mescroll-uni/mescroll-body.vue"
import MescrollUni from "@/components/mescroll-uni/mescroll-uni.vue"
Vue.component('mescroll-body', MescrollBody)
Vue.component('mescroll-uni', MescrollUni)

App.mpType = 'app'

//全局检查登录
Vue.prototype.checkLogin  = function(backpage, backtype){
	var NAME  = uni.getStorageSync('root_name');
	var PHONE = uni.getStorageSync('root_phone');
	if(NAME == '' || PHONE == ''){
		//console.log('需要跳转');
		uni.redirectTo({
			url:"../login/login?backpage="+backpage+"&backtype="+backtype
		});
		return false;
	}
	return [NAME, PHONE];
}

const app = new Vue({
    ...App
})
app.$mount()
