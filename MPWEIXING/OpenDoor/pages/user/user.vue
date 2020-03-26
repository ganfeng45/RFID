<template>
	<view class="content">
		<sl-filter :independence="false" :color="titleColor" :themeColor="themeColor" :menuList.sync="menuList" :topFixed="false" @result="result"></sl-filter>
		<view class="add_user">
			<image src="../../static/logo.png" mode="" @click="toadd()"></image>
		</view>
		<!-- <text>{{filterResult}}</text> -->
		<view class="userinfo" v-for="(user,i) in alluser" @click="userclick(user)">
			<view class="username">
				<label>姓名：</label> {{user.user_name}}
			</view>
			<view class="userphone">
				<label>电话：</label>{{user.user_phone}}
			</view>
			<view class="usernote">
				<label>备注：</label>{{user.user_notes}}
			</view>
		</view>
	</view>
</template>

<script>
//导入组件
import slFilter from '@/components/sl-filter/sl-filter.vue';
export default {
	components: {
		slFilter
	},
	data() {
		return {
			alluser: [],
			themeColor: '#000000',
			titleColor: '#666666',
			filterResult: '',
			menuList: [
				{
					title: '方向',
					detailTitle: '淮南 ⇌ 合肥（请选择方向）',
					isMutiple: false,
					key: 'direction',
					defaultSelectedIndex: 0,
					detailList: [
						{
							title: '淮南→合肥',
							value: ''
						},
						{
							title: '合肥→淮南',
							value: '1'
						}
					]
				},
				{
					title: '出发日期',
					key: 'goday',
					isMutiple: false,
					defaultSelectedIndex: 0,
					detailList: [
						{
							title: '今天',
							value: '0'
						},
						{
							title: '明天',
							value: '1'
						},
						{
							title: '后天',
							value: '2'
						},
						{
							title: '大后天',
							value: '3'
						}
					]
				},
				{
					title: '出发时间',
					detailTitle: '出发的具体时间',
					defaultSelectedIndex: 1,
					key: 'gotime',
					isMutiple: true,
					detailList: [
						{
							title: '不限',
							value: '233'
						},
						{
							title: '06：00',
							value: '6'
						},
						{
							title: '07：00',
							value: '7'
						},
						{
							title: '08：00',
							value: '8'
						},
						{
							title: '09：00',
							value: '9'
						},
						{
							title: '10：00',
							value: '10'
						},
						{
							title: '11：00',
							value: '11'
						},
						{
							title: '12：00',
							value: '12'
						},
						{
							title: '13：00',
							value: '13'
						},
						{
							title: '14：00',
							value: '14'
						},
					]
				}
			]
		};
	},
	onPullDownRefresh() {
		
	},
	onLoad() {
		var loginRes=this.checkLogin('../user/user',2);
		//载入页面后自动执行
		uni.request({
			url: 'https://www.auster.fun/doortp/public/api/door/alluser?token=api2020',
			method: 'GET',
			data: {},
			success: res => {
				this.alluser=res.data.data;
				//console.log(res.data.data);
			},
			fail: () => {},
			complete: () => {}
		});
	},
	onShow() {
		uni.request({
			url: 'https://www.auster.fun/doortp/public/api/door/alluser?token=api2020',
			method: 'GET',
			data: {},
			success: res => {
				this.alluser=res.data.data;
				//console.log(res.data.data);
			},
			fail: () => {},
			complete: () => {}
		});
	},
	methods: {
		toadd(){
			
				//console.log(item);
				uni.navigateTo({
				  url: `../adduser/adduser`
				});
				
		},
		userclick(item){
			console.log(item);
			uni.navigateTo({
			  url: `../userdatial/userdatial?item=${encodeURIComponent(
			    JSON.stringify(item)
			  )}`
			});
			
		},
		copyMessage(e) {
			//console.log(this.carres[e].fabuPrimaryData);
			uni.setClipboardData({
			    data: this.carres[e].fabuPrimaryData,
			    success: function () {
			        //console.log('success');
					uni.showToast({
						title: '情报已复制到系统剪贴板上~',
						duration: 1000,
						icon:'none'
					});
			    }
			});
		},
		callphone(e) {
			// console.log(e);
			console.log(this.carres[e].fabuPhonenumber);
			uni.makePhoneCall({
				phoneNumber: this.carres[e].fabuPhonenumber //仅为示例
			});
		},
		result(val) {
			console.log('filter_result:' + JSON.stringify(val));
			this.filterResult = JSON.stringify(val, null, 2);
			console.log(val.direction);
			uni.request({
				url: 'https://www.auster.fun/tp5/public/api/cardata/remessage?token=api2020',
				method: 'GET',
				data: {
					direction: val.direction,
					goday: val.goday,
					gotime: val.gotime
					//token: 'api2020'
				},
				success: res => {
					console.log('开始打印');
					console.log('开始打印');
					console.log('开始打印');
					console.log(res);
					this.carres = res.data.data;
					console.log('打印结束');
					console.log('打印结束');
					console.log('打印结束');
					uni.showToast({
						title: '更新成功',
						duration: 1000
					});
				},
				fail: () => {},
				complete: () => {}
			});
		}
	}
};
</script>

<style>
.*{
		margin: 0upx;
		padding: 0rpx;
	} 
	/* 大的背景 */
.content {
	width: 95%;
	margin: 1upx auto;
}
.userinfo{
	/* border-style: inset; */
	border-width: 5upx;
	border-radius:20upx;
	border: #000000 solid 1upx;
	padding: 10upx 20upx;
	margin: 10upx auto;
	margin-top: 20upx;
	background-color: #e2e2e2;
}
.username{
	border: #000000;
}
.username,.userphone{
	border-bottom: #000000 solid 1upx;
}
.username label,.userphone label,.usernote label{
	color: #616161;
	font-weight: bold;
	
}
/* 添加按钮 */
.add_user image{
	width: 70upx;
	height: 70upx;
	display: block;
	position:fixed;
	right:30upx;
	bottom:70upx;
	border-radius: 5upx;
}
 
</style>
