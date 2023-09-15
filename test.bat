ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [01].mkv" \
       -i "[Snow-Raws] Flower declaration of your heart season2 [01].ass" \
	   -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default \
	   "./output/[Snow-Raws] Flower declaration of your heart season2 [01].mkv"


ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [02].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [02].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [02].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [03].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [03].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [03].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [04].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [04].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [04].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [05].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [05].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [05].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [06].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [06].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [06].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [07].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [07].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [07].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [08].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [08].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [08].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [09].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [09].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [09].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [10].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [10].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [10].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [11].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [11].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [11].mkv"
ffmpeg -i "[Snow-Raws] Flower declaration of your heart season2 [12].mkv" -i "[Snow-Raws] Flower declaration of your heart season2 [12].ass" -vcodec hevc_nvenc -cq 25 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default "./output/[Snow-Raws] Flower declaration of your heart season2 [12].mkv"




#  輪廻のラグランジェ


多字幕軌使用

ffmpeg -i "Hayate the combat butler!! 26.mkv" \
       -map 0:0 -map 0:1 -map 0:3 
	   -vcodec hevc_nvenc -cq 28 
	   -acodec copy 
	   -scodec copy -disposition:s:0 default 
	   "./output/Hayate the combat butler!! 26.mkv"
