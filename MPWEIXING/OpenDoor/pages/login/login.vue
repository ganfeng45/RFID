<template>
	<view class="container">
		<view class="section"><input v-model="username" placeholder="用户名" /></view>
		<view class="section"><input v-model="password" password="true" placeholder="密码" /></view>
		<button class="login-btn" type="primary" @click="login">登录</button>
	</view>
</template>

<script>
	var pageoptions;
let _self;
export default {
	
	data() {
		return {
			username: '',
			password: ''
		};
	},
	methods: {
		onLoad:function(options){
			console.log('login_onload');
			console.log(options);
			pageoptions=options;
			console.log(pageoptions);
		},
		login() {
			const { username, password } = this;
			if (!username) {
				uni.showModal({
					content: '请填写用户名',
					showCancel: false
				});
				return;
			}
			if (!password) {
				uni.showModal({
					content: '请填写密码',
					showCancel: false
				});
				return;
			}
			uni.showLoading({
				title: '登录中...'
			});
			uni.request({
				url: 'https://www.auster.fun/doortp/public/api/member/login?token=api2020',
				method: 'POST',
				data: {
					user_name: username,
					user_pwd: password
				},
				success: res => {
					uni.hideLoading();
					console.log(res.data);
					if (res.data.code == 200) {
						uni.showToast({
							title: '登录成功',
							icon: 'none'
						});
						uni.setStorageSync('root_name', res.data.data.user_name);
						uni.setStorageSync('root_phone',res.data.data.user_phone);
						// 		uni.setStorageSync('username', res.result.username)
						if(pageoptions.backtype == 1){
							uni.redirectTo({url:pageoptions.backpage});
						}else{
							uni.switchTab({url:pageoptions.backpage});
						}
					}
					if(res.data.code == 400)
					{
						uni.showToast({
							title: res.data.msg,
							icon: 'none'
						});
					}
				},
				fail: () => {
					uni.hideLoading();
					uni.showModal({
						content:  '登录失败,请检查网络',
						showCancel: false
					})
				},
				complete: () => {}
			});

			// this.$cloud.callFunction({
			// 	name: 'login',
			// 	data: {
			// 		username,
			// 		password,
			// 	}
			// }).then((res) => {
			// 	uni.hideLoading()
			// 	if (res.result.token) {
			// 		uni.showToast({
			// 			title: '登录成功',
			// 			icon: 'none'
			// 		})
			// 		uni.setStorageSync('token', res.result.token)
			// 		uni.setStorageSync('username', res.result.username)
			// 		uni.switchTab({
			// 			url: '/pages/tabbar/member'
			// 		})
			// 	} else {
			// 		return Promise.reject(new Error(res.result.msg))
			// 	}
			// }).catch((err) => {
			// 	uni.hideLoading()
			// 	uni.showModal({
			// 		content: err.message || '登录失败',
			// 		showCancel: false
			// 	})
			// })
		}
	}
};
</script>

<style scoped>
.container {
	padding: 30px;
}

.section {
	margin: 20px;
	padding: 10px 0px;
	border-bottom: 2upx solid #eee;
}

.login-btn {
	margin: 30px 20px 0px 20px;
}
</style>
