<script>
export default {
	onLaunch: function() {
		console.log('App Launch');
	},
	onShow: function() {
		console.log('App Show');
	},
	onHide: function() {
		console.log('App Hide');
	}
};
</script>

<style>
/*每个页面公共css */
@import './common/uni.css';
/* 引入自定义图标库 */
@import './common/icon.css';
@import './common/icon2.css'; 
/* 引入动画库 */
@import './common/animate.css';
/* uchart */
@import './common/qiun.css';
</style>
