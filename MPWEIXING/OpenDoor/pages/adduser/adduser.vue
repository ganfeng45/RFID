<template>
	<view class="content">
		<view class="list">
			<view class="list-title">个人信息</view>
			<view class="list-item">
				<text>
					姓名
					<text class="color-red">*</text>
					：
				</text>
				<input v-model="user_name" value="" placeholder="请输入人员姓名" />
			</view>
			<view class="list-item">
				<text>证件类型：</text>
				<picker mode="selector" range-key="label" :range="idTypeList" @change="idTypeChange">
					<view class="xiala">
						<text>{{ idTypeList[id_type].label }}</text>
						<image src="/static/xl.png"></image>
					</view>
				</picker>
			</view>
			<view class="list-item">
				<text>证件uid：</text>
				<input :style="{ backgroundColor: isCn ? '#EE6A50' : '' }" v-model="user_uid" placeholder="UID" type="text" />
				<button size="mini" type="primary" class="UIDbutton" @tap="getUID"> <view class="uidgetTXT">
					
				{{uidTXT}}</view> </button>
			</view>
			<view class="list-item">
				<text>
					性别
					<text class="color-red">*</text>
					：
				</text>
				<radio-group @change="sexChange">
					<label>
						<radio value="1" :checked="sex == '1'" />
						<text>男</text>
						<radio value="0" :checked="sex == '0'" />
						<text>女</text>
						<radio value="3" :checked="sex == '3'" />
						<text>未知</text>
					</label>
				</radio-group>
			</view>
			<view class="list-item">
				<text>
					联系电话
					<text class="color-red">*</text>
					：
				</text>
				<input v-model="user_phone" value="" placeholder="请输入联系电话" />
			</view>
			<view class="list-item">
				<text>备注：</text>
				<input v-model="user_notes" placeholder="请输入备注信息" />
			</view>
		</view>
		<view class="list">
			<view class="list-title">部门信息</view>
			<view class="list-item">
				<text>
					部门信息
					<text class="color-red">*</text>
					：
				</text>
				<radio-group @change="bodyStatusChange">
					<label>
						<radio value="0" />
						<text>开发部</text>
					</label>
					<label>
						<radio value="1" />
						<text>人事部</text>
					</label>
					<label>
						<radio value="2" />
						<text>财务部</text>
					</label>
					<label>
						<radio value="3" />
						<text>后勤部</text>
					</label>
					<label>
						<radio value="4" />
						<text>X部</text>
					</label>
					<label>
						<radio value="99" />
						<text>未定</text>
					</label>
				</radio-group>
			</view>
			<view class="list-item">
				<text>
					门卡开始时间
					<text class="color-red">*</text>
					：
				</text>
				<picker mode="date" @change="bodyStatusDateChangeSTART">
					<button size="mini">{{ bodyStatusDateSTART || '请选择日期' }}</button>
				</picker>
				<picker mode="time" @change="bodyStatusTimeChangeSTART">
					<button size="mini">{{ bodyStatusTimeSTART || '请选择时间' }}</button>
				</picker>
			</view>
			<view class="list-item">
				<text>
					门卡到期时间
					<text class="color-red">*</text>
					：
				</text>
				<picker mode="date" @change="bodyStatusDateChange">
					<button size="mini">{{ bodyStatusDate || '请选择日期' }}</button>
				</picker>
				<picker mode="time" @change="bodyStatusTimeChange">
					<button size="mini">{{ bodyStatusTime || '请选择时间' }}</button>
				</picker>
			</view>

			<view class="divider"></view>

			<button @click="submit">提交信息</button>
			<button type="primary" @click="redata">上传数据</button>
		</view>
	</view>
</template>

<script>
export default {
	data() {
		return {
			data: [],
			user_name: '',
			idTypeList: [
				{
					label: '普通卡',
					value: 0
				},
				{
					label: 'root用户卡',
					value: 1
				}
			],
			id_type: 0,
			user_uid: '',
			isCn: false,
			user_sex: -1,
			user_phone: '',
			user_notes: '',
			user_part: -1,
			bodyStatusTimeSTART: '', //门卡开始时间
			bodyStatusDateSTART: '', //门卡开始日期
			bodyStatusTime: '', //门卡到期时间
			bodyStatusDate: '', //门卡到期日期
			user_startTIME: '',
			user_deadTIME: '',
			uidTXT:'GET',
			timer: '',
			value: 60,
		};
	},
	computed: {
		check_in_time: function() {
			return new Date(this.checkInTimeStr).getTime();
		}
	},
	watch: {
		bodyStatusDateSTART() {
			this.getBodyStatusTimeStampSTART();
		},
		bodyStatusTimeSTART() {
			this.getBodyStatusTimeStampSTART();
		},
		bodyStatusDate() {
			this.getBodyStatusTimeStamp();
		},
		bodyStatusTime() {
			this.getBodyStatusTimeStamp();
		},
		id_type() {
			this.parseIdCard();
		},
		user_uid() {
			this.parseIdCard();
		}
	},
	onLoad(e) {
		// 占位符
	},
	methods: {
		getUID(){
			this.timer = setInterval(this.get, 1000);
		},
		beforeDestroy() {
			clearInterval(this.timer);
		},
		get() {
			uni.request({
				url: 'https://www.auster.fun/doortp/public/api/door/giveuid?token=api2020',
				method: 'POST',
				data: {},
				success: res => {
					console.log('成功');
					console.log(res);
					if(res.data.code==400){
						this.value --;
						console.log(this.value);
						this.uidTXT=this.value;
					}
					if(res.data.code==200){
						this.uidTXT='OK';
						this.user_uid=res.data.data.readUID;
						uni.showToast({
							title: 'UID获取成功',
							duration: 1000,
							complete() {}
						});
						console.log(res.data.data);
						console.log(res.data.data.readUID);
						clearInterval(this.timer);
						console.log('定时器销毁');
					}
					if(res.data.code==300)
					{
						clearInterval(this.timer);
						console.log('定时器销毁');
						this.value=60;
						this.uidTXT='GET';
						uni.showModal({
						    title: '错误',
						    content: res.data.msg,
							showCancel:false,
							success: function (res) {
							        if (res.confirm) {
										
							        } 
							    }
						});
					}
					if(this.value<=0){
						clearInterval(this.timer);
						console.log('定时器销毁');
					}
					// clearInterval(this.timer);
					// console.log('定时器销毁');
				},
				fail: () => {},
				complete: () => {}
			});
		},
		parseIdCard() {
			//分析身份证
			if (this.id_type == 0) {
				this.isCn = /[^\w\.\/]/.test(this.user_uid);
				if (this.user_uid.length == 18) {
					let year = parseInt(this.user_uid.substr(6, 4));
					let mark = parseInt(this.user_uid.substr(16, 1));
					if (!isNaN(year)) {
						this.age = new Date().getFullYear() - year;
					}
					if (!isNaN(mark)) {
						this.user_sex = mark % 2;
					}
					console.log(this.user_sex);
				}
			}
		},
		//证件类型
		idTypeChange(e) {
			this.id_type = e.detail.value;
		},
		//性别选择
		sexChange(e) {
			this.user_sex = e.detail.value;
		},

		//部门信息选择
		bodyStatusChange(e) {
			this.user_depart = e.detail.value;
		},
		//请选择日期
		bodyStatusDateChange(e) {
			this.bodyStatusDate = e.detail.value;
		},
		//请选择时间
		bodyStatusTimeChange(e) {
			this.bodyStatusTime = e.detail.value;
		},
		//请选择日期
		bodyStatusDateChangeSTART(e) {
			this.bodyStatusDateSTART = e.detail.value;
		},
		//请选择时间
		bodyStatusTimeChangeSTART(e) {
			this.bodyStatusTimeSTART = e.detail.value;
		},
		getBodyStatusTimeStamp() {
			if (this.bodyStatusDate && this.bodyStatusTime) {
				this.user_deadTIME = new Date(this.bodyStatusDate + ' ' + this.bodyStatusTime).getTime();
				this.user_deadTIME = this.user_deadTIME / 1000;
			} else {
				this.body_deadTIME = 0;
			}
		},
		getBodyStatusTimeStampSTART() {
			if (this.bodyStatusDateSTART && this.bodyStatusTimeSTART) {
				this.user_startTIME = new Date(this.bodyStatusDateSTART + ' ' + this.bodyStatusTimeSTART).getTime();
				this.user_startTIME = this.user_startTIME / 1000;
			} else {
				this.user_startTIME = 0;
			}
		},
		redata() {
			uni.request({
				url: 'https://www.auster.fun/doortp/public/api/door/readata?token=api2020',
				method: 'POST',
				data: {
					user_name: this.user_name,
					id_type: this.id_type,
					user_uid: this.user_uid,
					user_sex: this.user_sex,
					native: this.native,
					user_phone: this.user_phone,
					user_notes: this.user_notes,
					user_startTIME: this.user_startTIME,
					user_deadTIME: this.user_deadTIME
				},
				success: res => {
					console.log('上传的数据是：');
					console.log(res);
				},
				fail: () => {},
				complete: () => {}
			});
		},
		//提交函数
		submit() {
			if (!this.user_name) {
				uni.showModal({
					content: '请填写人员姓名',
					showCancel: false
				});
				return false;
			}
			if (!this.user_sex == -1) {
				uni.showModal({
					content: '请填写人员性别',
					showCancel: false
				});
				return false;
			}

			if (!this.user_phone || this.user_phone.length !== 11) {
				uni.showModal({
					content: '请正确填写人员手机号',
					showCancel: false
				});
				return false;
			}
			if (!this.user_depart) {
				uni.showModal({
					content: '请选择人员部门',
					showCancel: false
				});
				return false;
			}
			if (!this.bodyStatusTimeSTART || !this.bodyStatusDateSTART) {
				uni.showModal({
					content: '请设置开始 日期 与 时间',
					showCancel: false
				});
				return false;
			}
			if (!this.bodyStatusDate || !this.bodyStatusTime) {
				uni.showModal({
					content: '请设置过期 日期 与 时间',
					showCancel: false
				});
				return false;
			}
			console.log(this.user_startTIME);
			console.log(this.user_deadTIME);
			uni.request({
				url: 'https://www.auster.fun/doortp/public/api/door/upuserdata?token=api2020',
				method: 'POST',
				data: {
					user_name: this.user_name,
					id_type: this.id_type,
					user_uid: this.user_uid,
					user_sex: this.user_sex,
					native: this.native,
					user_phone: this.user_phone,
					user_notes: this.user_notes,
					user_startTIME: this.user_startTIME,
					user_deadTIME: this.user_deadTIME
				},
				success: res => {
					console.log('上传的数据是：');
					console.log(res);
					if (res.data.code == 200) {
						uni.showToast({
							title: '人员添加成功',
							duration: 1000,
							complete() {}
						});
						setTimeout(function() {
							uni.navigateBack({
								delta: 1
							});
						}, 1800);

						console.log('准备跳转');
						console.log('跳转');
					}
				},
				fail: () => {},
				complete: () => {}
			});
			// uni.showModal({
			// 	content: '请重新提交',
			// 	showCancel: false
			// });
			// uni.showLoading({
			// 	title: '上传中...'
			// });
		}
	}
};
</script>

<style>
page {
	background: #f0f0f0;
}

.content {
	display: flex;
	flex-direction: column;
	align-items: center;
	justify-content: center;
	margin-bottom: 50rpx;
}

.list {
	display: flex;
	flex-direction: column;
	width: 680rpx;
	margin: auto;
	margin-top: 20rpx;
	background: #ffffff;
	border-radius: 20rpx;
	padding-bottom: 20rpx;
}

.list-title {
	width: 580rpx;
	padding: 20rpx;
	display: flex;
	font-size: 40rpx;
	font-weight: bold;
	border-bottom: #f0f0f0 1rpx solid;
	margin: auto;
	margin-bottom: 20rpx;
}

.list-item {
	width: 580rpx;
	padding: 20rpx;
	display: flex;
	margin: auto;
	font-size: 30rpx;
	align-items: center;
}

.list-item > text {
	white-space: nowrap;
}

.list-item input {
	background: #f0f0f0;
	border-radius: 10rpx;
	padding-left: 20rpx;
	padding-right: 20rpx;
	width: 300rpx;
	height: 60rpx;
	color: #999999;
	font-size: 30rpx;
}

.list-item slider {
	width: 300rpx;
}

.list-item picker {
	margin-right: 10px;
}

.list-item picker button {
	vertical-align: middle;
}

.list-item label {
	display: inline-block;
	margin: 5px 0px;
}

.list-item label text {
	margin-right: 10px;
}

.list-item textarea {
	border-radius: 5px;
	background-color: #f8f8f8;
	padding: 10px;
}

.xiala {
	display: flex;
	background: #f0f0f0;
	padding: 10rpx;
	border-radius: 10rpx;
	color: #999999;
	font-size: 30rpx;
}

.xiala image {
	display: flex;
	padding-top: 5rpx;
	width: 30rpx;
	height: 30rpx;
	margin-left: 10rpx;
}

.divider {
	content: '';
	width: 580rpx;
	margin: 5px auto;
	height: 1px;
	background-color: #dddddd;
}

.color-red {
	color: #ff3333;
}
/* uid获取按钮*/
.UIDbutton{
	height: 65upx;
	/* width: 100upx; */
	margin: 0 0;
	padding: 0 0;
	font-size: 10upx;
}
.uidgetTXT{
	
}
</style>
